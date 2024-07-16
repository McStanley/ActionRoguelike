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
	UPROPERTY(EditAnywhere, Category = "Explosion")
	UParticleSystem* EmitterTemplate;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
