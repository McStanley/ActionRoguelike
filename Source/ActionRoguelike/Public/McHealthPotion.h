// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McRespawningPickUp.h"
#include "McHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcHealthPotion : public AMcRespawningPickUp
{
	GENERATED_BODY()

public:
	AMcHealthPotion();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsCost;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
