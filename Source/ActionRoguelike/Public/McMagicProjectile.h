// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McProjectile.h"
#include "McMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AMcMagicProjectile : public AMcProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMcMagicProjectile();

protected:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* LoopSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	UParticleSystem* EmitterTemplate;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);

	void Explode();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
};
