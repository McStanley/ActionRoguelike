// Fill out your copyright notice in the Description page of Project Settings.


#include "McRespawningPickUp.h"

// Sets default values
AMcRespawningPickUp::AMcRespawningPickUp()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	RespawnDelay = 5.0f;

	bReplicates = true;
}

void AMcRespawningPickUp::Deactivate()
{
	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &AMcRespawningPickUp::Activate, RespawnDelay);
}

void AMcRespawningPickUp::Activate()
{
	MeshComp->SetVisibility(true, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}
