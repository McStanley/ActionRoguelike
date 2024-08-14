// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "McGameModeBase.generated.h"

class UMcBotData;
class UMcSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

namespace EEnvQueryStatus
{
	enum Type : int;
}

USTRUCT(BlueprintType)
struct FBotInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	FBotInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 1;
		KillReward = 1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId BotDataId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 KillReward;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMcGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

protected:
	FString SaveGameSlotName;

	UPROPERTY()
	UMcSaveGame* SaveGame = nullptr;

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

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int32 TeleportPickUpsQuantity;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSubclassOf<AActor> TeleportPickUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* BotTypesTable;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float BotSpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* BotSpawnQuery = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve = nullptr;

	// UPROPERTY(EditDefaultsOnly, Category = "AI")
	// TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 KillAwardCredits;

	UFUNCTION()
	void OnPickUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnBotSpawnTimerElapsed();

	UFUNCTION()
	void OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnBotDataLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION(Exec)
	void KillAllAI();

	void LoadSaveGame();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
