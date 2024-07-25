// Fill out your copyright notice in the Description page of Project Settings.


#include "McGameModeBase.h"

#include "EngineUtils.h"
#include "McAttributeComponent.h"
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

		UMcAttributeComponent* AttributeComp = Cast<UMcAttributeComponent>(
			Bot->GetComponentByClass(UMcAttributeComponent::StaticClass())
		);

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