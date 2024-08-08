// Fill out your copyright notice in the Description page of Project Settings.


#include "McRespawningPickUp.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMcRespawningPickUp::AMcRespawningPickUp()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	bActive = true;
	RespawnDelay = 5.0f;

	bReplicates = true;
}

void AMcRespawningPickUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMcRespawningPickUp, bActive);
}

void AMcRespawningPickUp::Deactivate()
{
	bActive = false;
	HandleDeactivate();

	GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &AMcRespawningPickUp::Activate, RespawnDelay);
}

void AMcRespawningPickUp::Activate()
{
	bActive = true;
	HandleActivate();
}

void AMcRespawningPickUp::OnRep_Active()
{
	if (bActive)
	{
		HandleActivate();
	}
	else
	{
		HandleDeactivate();
	}
}

void AMcRespawningPickUp::HandleDeactivate()
{
	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AMcRespawningPickUp::HandleActivate()
{
	MeshComp->SetVisibility(true, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}
