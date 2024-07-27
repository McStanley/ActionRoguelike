// Fill out your copyright notice in the Description page of Project Settings.


#include "McGameModeBase.h"

#include "EngineUtils.h"
#include "McAttributeComponent.h"
#include "McCharacter.h"
#include "AI/McAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

AMcGameModeBase::AMcGameModeBase()
{
	SpawnTimerInterval = 2.f;
}

void AMcGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMcGameModeBase::OnSpawnTimerElapsed,
	                                SpawnTimerInterval, true);
}

void AMcGameModeBase::OnSpawnTimerElapsed()
{
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMcGameModeBase::OnQueryCompleted);
	}
}

void AMcGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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
