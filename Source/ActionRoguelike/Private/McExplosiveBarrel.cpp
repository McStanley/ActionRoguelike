// Fill out your copyright notice in the Description page of Project Settings.


#include "McExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AMcExplosiveBarrel::AMcExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMcExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AMcExplosiveBarrel::Explode);
}

// Called every frame
void AMcExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMcExplosiveBarrel::Explode(UPrimitiveComponent* HitComponent,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse,
                                 const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}
