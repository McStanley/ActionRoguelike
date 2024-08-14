// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "McBotData.generated.h"

class UMcAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMcBotData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TArray<TSubclassOf<UMcAction>> Actions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
