// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "McRespawningPickUp.h"
#include "McActionPickUp.generated.h"

class UMcAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcActionPickUp : public AMcRespawningPickUp
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<UMcAction> ActionClass;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
