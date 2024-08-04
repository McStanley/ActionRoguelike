// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "McGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

namespace EEnvQueryStatus
{
	enum Type : int;
}

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMcGameModeBase();

	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	UEnvQuery* PickUpQuery = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int32 CoinsQuantity;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int32 HealthPotionsQuantity;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSubclassOf<AActor> HealthPotionClass;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float BotSpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* BotSpawnQuery = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 KillAwardCredits;

	UFUNCTION()
	void OnPickUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnBotSpawnTimerElapsed();

	UFUNCTION()
	void OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION(Exec)
	void KillAllAI();
};
