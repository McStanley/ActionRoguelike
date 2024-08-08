// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "McRespawningPickUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AMcRespawningPickUp : public AActor, public IMcGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMcRespawningPickUp();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	FTimerHandle TimerHandle_Reactivate;

	float RespawnDelay;

	UPROPERTY(ReplicatedUsing="OnRep_Active")
	bool bActive;

	void Deactivate();

	void Activate();

	UFUNCTION()
	void OnRep_Active();

	void HandleDeactivate();

	void HandleActivate();
};
