// Fill out your copyright notice in the Description page of Project Settings.


#include "McGameModeBase.h"

#include "EngineUtils.h"
#include "McAttributeComponent.h"
#include "McCharacter.h"
#include "McPlayerState.h"
#include "AI/McAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(
	TEXT("mc.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat
);

AMcGameModeBase::AMcGameModeBase()
{
	CoinsQuantity = 3;
	HealthPotionsQuantity = 4;

	BotSpawnTimerInterval = 2.f;
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

	const int32 ItemsQuantity = CoinsQuantity + HealthPotionsQuantity;

	int32 CoinsSpawned = 0;
	int32 HealthPotionsSpawned = 0;

	while (Indices.Num() < ItemsQuantity && Indices.Num() < LocationsSize)
	{
		int32 RandIndex = FMath::RandRange(0, LocationsSize - 1);

		if (Indices.Contains(RandIndex)) continue;

		Indices.Add(RandIndex);

		const bool bAllCoinsSpawned = CoinsSpawned >= CoinsQuantity;
		const bool bAllHealthPotionsSpawned = HealthPotionsSpawned >= HealthPotionsQuantity;

		const bool bPreferCoin = FMath::RandBool();

		const bool bSpawnCoin = (!bAllCoinsSpawned && bPreferCoin) || bAllHealthPotionsSpawned;

		// UE_LOG(LogTemp, Type::Display, TEXT("bSpawnCoin: %d"), bSpawnCoin);

		TSubclassOf<AActor> ClassToSpawn = bSpawnCoin ? CoinClass : HealthPotionClass;
		int32* XSpawned = bSpawnCoin ? &CoinsSpawned : &HealthPotionsSpawned;

		FVector Location = Locations[RandIndex];

		// Adjust health potion vertical offset
		if (!bSpawnCoin)
		{
			Location += FVector(0.f, 0.f, -30.f);
		}

		GetWorld()->SpawnActor<AActor>(ClassToSpawn, Location, FRotator::ZeroRotator);

		*XSpawned += 1;;
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

	if (bValidResult)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator, SpawnParams);

		DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Blue, false, 15.f);
	}
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
				State->AddCredits(VictimActor, 1);
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
