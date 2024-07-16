// Fill out your copyright notice in the Description page of Project Settings.


#include "McMagicProjectile.h"

#include "McAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMcMagicProjectile::AMcMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMcMagicProjectile::OnSphereOverlap);

	MovementComp->InitialSpeed = 2000.f;
}

void AMcMagicProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UMcAttributeComponent* AttributeComp = Cast<UMcAttributeComponent>(
			OtherActor->GetComponentByClass(UMcAttributeComponent::StaticClass())
		);

		if (AttributeComp != nullptr)
		{
			AttributeComp->ApplyHealthChange(-20.f);

			this->Destroy();
		}
	}
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
