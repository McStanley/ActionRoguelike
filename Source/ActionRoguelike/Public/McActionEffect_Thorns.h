// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McActionEffect.h"
#include "McActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMcActionEffect_Thorns : public UMcActionEffect
{
	GENERATED_BODY()

	UMcActionEffect_Thorns();

public:
	virtual bool CanStart_Implementation(AActor* Instigator) override;

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

private:
	UFUNCTION()
	void OnOwnerHealthChanged(AActor* InstigatorActor, UMcAttributeComponent* OwningComp, float NewHealth,
	                          float Delta, bool bReflected);
};
