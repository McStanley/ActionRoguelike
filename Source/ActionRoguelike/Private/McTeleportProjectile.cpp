// Fill out your copyright notice in the Description page of Project Settings.


#include "McTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMcTeleportProjectile::AMcTeleportProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplodeDelay = 0.2f;
	TeleportDelay = 0.2f;
	DestroyDelay = 0.5f;

	MovementComp->ProjectileGravityScale = 0;
	MovementComp->InitialSpeed = 3000.f;
}

void AMcTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &AMcTeleportProjectile::OnHit);
}

// Called when the game starts or when spawned
void AMcTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(this->GetInstigator(), true);

	GetWorldTimerManager().SetTimer(TimerHandle_StartExplode, this, &AMcTeleportProjectile::Explode, ExplodeDelay);
}

// Called every frame
void AMcTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMcTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	MovementComp->Deactivate();

	GetWorldTimerManager().ClearTimer(TimerHandle_StartExplode);

	Explode();
}


void AMcTeleportProjectile::Explode()
{
	if (MovementComp->IsActive())
	{
		MovementComp->Deactivate();
	}

	EffectComp->SetTemplate(EnterParticle);

	GetWorldTimerManager().SetTimer(TimerHandle_FinishExplode, this, &AMcTeleportProjectile::Teleport, TeleportDelay);
}

void AMcTeleportProjectile::Teleport()
{
	APawn* Player = GetInstigator();
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	SphereComp->SetCollisionProfileName(TEXT("NoCollision"));
	EffectComp->SetTemplate(ExitParticle);

	Player->TeleportTo(Location, Rotation);

	GetWorldTimerManager().SetTimer(TimerHandle_DestroySelf, this, &AMcTeleportProjectile::DestroySelf, DestroyDelay);
}

void AMcTeleportProjectile::DestroySelf()
{
	this->Destroy();
}
