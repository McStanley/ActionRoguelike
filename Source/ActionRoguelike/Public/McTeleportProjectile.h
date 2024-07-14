// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McProjectile.h"
#include "McTeleportProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API AMcTeleportProjectile : public AMcProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMcTeleportProjectile();

protected:
	FTimerHandle TimerHandle_StartExplode;
	FTimerHandle TimerHandle_FinishExplode;
	FTimerHandle TimerHandle_DestroySelf;

	float ExplodeDelay;
	float TeleportDelay;
	float DestroyDelay;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* EnterParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* ExitParticle = nullptr;

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent,
	           AActor* OtherActor,
	           UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse,
	           const FHitResult& Hit);

	void Explode();
	void Teleport();
	void DestroySelf();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
