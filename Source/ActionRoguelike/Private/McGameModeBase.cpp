// Fill out your copyright notice in the Description page of Project Settings.


#include "McGameModeBase.h"

#include "EngineUtils.h"
#include "McActionComponent.h"
#include "McAttributeComponent.h"
#include "McBotData.h"
#include "McCharacter.h"
#include "McGameplayInterface.h"
#include "McPlayerState.h"
#include "McSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/McAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
	TEXT("mc.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat
);

AMcGameModeBase::AMcGameModeBase()
{
	SaveGameSlotName = "McSaveGame01";

	CoinsQuantity = 3;
	HealthPotionsQuantity = 4;
	TeleportPickUpsQuantity = 2;

	BotSpawnTimerInterval = 2.f;

	KillAwardCredits = 1;
}

void AMcGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveName = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveName.Len() > 0)
	{
		SaveGameSlotName = SelectedSaveName;
	}

	LoadSaveGame();
}

void AMcGameModeBase::StartPlay()
{
	Super::StartPlay();

	UEnvQueryInstanceBlueprintWrapper* PickUpQueryInstance = UEnvQueryManager::RunEQSQuery(
		this, PickUpQuery, this, EEnvQueryRunMode::AllMatching, nullptr
	);

	if (ensure(PickUpQueryInstance))
	{
		PickUpQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMcGameModeBase::OnPickUpQueryCompleted);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMcGameModeBase::OnBotSpawnTimerElapsed,
	                                BotSpawnTimerInterval, true);
}

void AMcGameModeBase::OnPickUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                             EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Type::Warning, TEXT("PickUp EQS query failed!"))
		return;
	}

	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	const int32 LocationsSize = Locations.Num();

	if (!LocationsSize)
	{
		UE_LOG(LogTemp, Type::Warning, TEXT("No valid pick up locations found!"));
		return;
	}

	TArray<int> Indices;

	struct FPickUp
	{
		TSubclassOf<AActor> Class;
		int32 RequestedQuantity;
		int32 CurrentQuantity;
		FVector3d Offset;
	};

	FPickUp PickUps[] = {
		{CoinClass, CoinsQuantity, 0, FVector3d::ZeroVector},
		{HealthPotionClass, HealthPotionsQuantity, 0, FVector3d(0.f, 0.f, -30.f)},
		{TeleportPickUpClass, TeleportPickUpsQuantity, 0, FVector3d::ZeroVector}
	};
	constexpr int32 PickUpsLength = sizeof(PickUps) / sizeof(FPickUp);


	int32 RequestedQuantity = 0;

	for (int i = 0; i < PickUpsLength; i++)
	{
		RequestedQuantity += PickUps[i].RequestedQuantity;
	}

	while (Indices.Num() < RequestedQuantity && Indices.Num() < LocationsSize)
	{
		int32 LocationIndex = FMath::RandRange(0, LocationsSize - 1);

		if (Indices.Contains(LocationIndex)) continue;

		Indices.Add(LocationIndex);

		FPickUp* SelectedPickUp;

		do
		{
			const int32 PickUpIndex = FMath::RandRange(0, PickUpsLength - 1);
			SelectedPickUp = &PickUps[PickUpIndex];
		}
		while (SelectedPickUp->CurrentQuantity >= SelectedPickUp->RequestedQuantity);

		TSubclassOf<AActor> ClassToSpawn = SelectedPickUp->Class;

		FVector Location = Locations[LocationIndex];
		Location += SelectedPickUp->Offset;

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, Location, FRotator::ZeroRotator);

		SelectedPickUp->CurrentQuantity += 1;;
	}
}

void AMcGameModeBase::OnBotSpawnTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled: CVarSpawnBots."));
		return;
	}

	int32 BotsAliveCount = 0;

	for (TActorIterator<AMcAICharacter> It(GetWorld()); It; ++It)
	{
		AMcAICharacter* Bot = *It;

		UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(Bot);

		if (ensure(AttributeComp) && AttributeComp->IsALive())
		{
			BotsAliveCount++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), BotsAliveCount);

	float MaxBotCount = 10;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (BotsAliveCount >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn..."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, BotSpawnQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr
	);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMcGameModeBase::OnBotQueryCompleted);
	}
}

void AMcGameModeBase::OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                          EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Type::Warning, TEXT("Bot spawn EQS query failed!"))
		return;
	}

	TArray<FVector> Locations;
	const bool bValidResult = QueryInstance->GetQueryResultsAsLocations(Locations);

	if (bValidResult && BotTypesTable)
	{
		TArray<FBotInfoRow*> Rows;
		BotTypesTable->GetAllRows("", Rows);

		int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
		FBotInfoRow* RandomRow = Rows[RandomIndex];

		if (UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			LogOnScreen(this, "Loading bot...", FColor::Green);

			TArray<FName> Bundles;
			FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
				this, &AMcGameModeBase::OnBotDataLoaded, RandomRow->BotDataId, Locations[0]
			);

			AssetManager->LoadPrimaryAsset(RandomRow->BotDataId, Bundles, Delegate);
		}
	}
}

void AMcGameModeBase::OnBotDataLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading bot.", FColor::Green);

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (AssetManager == nullptr) return;

	UMcBotData* BotData = AssetManager->GetPrimaryAssetObject<UMcBotData>(LoadedId);
	if (BotData == nullptr) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewBot = GetWorld()->SpawnActor<AActor>(BotData->BotClass, SpawnLocation,
	                                                FRotator::ZeroRotator, SpawnParams);

	if (NewBot)
	{
		LogOnScreen(
			this, FString::Printf(TEXT("Spawned bot: %s (%s)"),
			                      *GetNameSafe(NewBot),
			                      *GetNameSafe(BotData)
			));

		UMcActionComponent* ActionComp = NewBot->GetComponentByClass<UMcActionComponent>();
		if (ActionComp)
		{
			for (TSubclassOf<UMcAction> ActionClass : BotData->Actions)
			{
				ActionComp->AddAction(NewBot, ActionClass);
			}
		}
	}

	DrawDebugSphere(GetWorld(), SpawnLocation, 50.f, 20, FColor::Blue, false, 15.f);
}

void AMcGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	if (const AMcCharacter* Player = Cast<AMcCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate TimerDelegate_RespawnPlayer;
		TimerDelegate_RespawnPlayer.BindUFunction(this, FName("RespawnPlayerElapsed"), Player->GetController());

		constexpr float RespawnDelay = 3.5f;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, TimerDelegate_RespawnPlayer, RespawnDelay, false);
	}

	if (const AMcCharacter* Player = Cast<AMcCharacter>(KillerActor))
	{
		if (const AMcAICharacter* Bot = Cast<AMcAICharacter>(VictimActor))
		{
			if (AMcPlayerState* State = Player->GetPlayerStateChecked<AMcPlayerState>())
			{
				State->AddCredits(VictimActor, KillAwardCredits);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer %s"),
	       *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));
}

void AMcGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (!ensure(Controller)) return;

	Controller->UnPossess();

	RestartPlayer(Controller);
}

void AMcGameModeBase::KillAllAI()
{
	for (TActorIterator<AMcAICharacter> It(GetWorld()); It; ++It)
	{
		AMcAICharacter* Bot = *It;

		UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(Bot);

		if (ensure(AttributeComp) && AttributeComp->IsALive())
		{
			AttributeComp->Kill(this);
		}
	}
}

void AMcGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		SaveGame = Cast<UMcSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));

		if (SaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UMcGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : SaveGame->SavedActors)
			{
				if (ActorData.Name == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemoryReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
					Ar.ArIsSaveGame = true;

					Actor->Serialize(Ar);

					IMcGameplayInterface::Execute_OnActorStateLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		SaveGame = Cast<UMcSaveGame>(UGameplayStatics::CreateSaveGameObject(UMcSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame object."));
	}
}

void AMcGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AMcPlayerState* PlayerState = Cast<AMcPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->Save(SaveGame);
			break;
		}
	}

	SaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UMcGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.Name = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		Ar.ArIsSaveGame = true;

		Actor->Serialize(Ar);

		SaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGameSlotName, 0);
}

void AMcGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AMcPlayerState* PlayerState = NewPlayer->GetPlayerState<AMcPlayerState>();
	if (PlayerState)
	{
		PlayerState->Load(SaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
