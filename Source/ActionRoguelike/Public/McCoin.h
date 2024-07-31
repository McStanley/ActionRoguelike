// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McRespawningPickUp.h"
#include "McCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcCoin : public AMcRespawningPickUp
{
	GENERATED_BODY()

public:
	AMcCoin();

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
