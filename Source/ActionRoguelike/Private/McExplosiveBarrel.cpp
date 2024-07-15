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
	StaticMeshComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->SetupAttachment(RootComponent);
}

void AMcExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AMcExplosiveBarrel::Explode);
}

// Called when the game starts or when spawned
void AMcExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
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

	UE_LOG(LogTemp, Log, TEXT("Explosion (Explosive Barrel C++)"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	const FString DebugString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugString, nullptr, FColor::Green, 2.f, true);
}
