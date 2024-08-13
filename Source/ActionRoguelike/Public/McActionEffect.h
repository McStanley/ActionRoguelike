// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McAction.h"
#include "McActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMcActionEffect : public UMcAction
{
	GENERATED_BODY()

public:
	UMcActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Effect")
	float GetTimeRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "Effect")
	float GetTimePercentRemaining() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
