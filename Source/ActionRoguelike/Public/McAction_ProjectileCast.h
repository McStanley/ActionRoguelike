// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McAction.h"
#include "McAction_ProjectileCast.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMcAction_ProjectileCast : public UMcAction
{
	GENERATED_BODY()

public:
	UMcAction_ProjectileCast();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	UParticleSystem* CastingEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	UAnimMontage* CastingAnim = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	float SpawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	float MaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	TSubclassOf<AActor> ProjectileClass;

	UFUNCTION()
	void TimerCallback(ACharacter* Character);

	FTransform GetProjectileSpawnTM(const ACharacter* Character) const;
};
