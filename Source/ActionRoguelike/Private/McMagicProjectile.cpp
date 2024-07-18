// Fill out your copyright notice in the Description page of Project Settings.


#include "McMagicProjectile.h"

#include "McAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMcMagicProjectile::AMcMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMcMagicProjectile::OnSphereOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &AMcMagicProjectile::OnSphereHit);

	MovementComp->InitialSpeed = 2000.f;
}

void AMcMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AudioComp->SetSound(LoopSound);
}

// Called when the game starts or when spawned
void AMcMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(AudioComp->GetSound()))
	{
		AudioComp->Play();
	}
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
		}

		Explode();
	}
}

void AMcMagicProjectile::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AMcMagicProjectile::Explode()
{
	if (ensure(EmitterTemplate))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, GetActorLocation(),
		                                         GetActorRotation());
	}

	if (ensure(ImpactSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}
