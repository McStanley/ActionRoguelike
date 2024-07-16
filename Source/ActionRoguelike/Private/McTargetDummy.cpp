// Fill out your copyright notice in the Description page of Project Settings.


#include "McTargetDummy.h"

#include "McAttributeComponent.h"

// Sets default values
AMcTargetDummy::AMcTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UMcAttributeComponent>("AttributeComp");
}

void AMcTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AMcTargetDummy::OnHealthChanged);
}


void AMcTargetDummy::OnHealthChanged(AActor* InstigatorActor, UMcAttributeComponent* OwningComp, float NewHealth,
                                     float Delta)
{
	if (Delta < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
