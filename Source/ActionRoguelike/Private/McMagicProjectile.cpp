// Fill out your copyright notice in the Description page of Project Settings.


#include "McMagicProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMcMagicProjectile::AMcMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComp->InitialSpeed = 2000.f;
}

// Called when the game starts or when spawned
void AMcMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMcMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
