// Fill out your copyright notice in the Description page of Project Settings.


#include "McAction_ProjectileCast.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UMcAction_ProjectileCast::UMcAction_ProjectileCast()
{
	SpawnDelay = 0.2f;
	SocketName = "Muzzle_01";
	MaxDistance = 10000.f;
}

void UMcAction_ProjectileCast::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character == nullptr) return;

	if (ensure(CastingEffect))
	{
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), SocketName);
	}

	if (ensure(CastingAnim))
	{
		Character->PlayAnimMontage(CastingAnim);
	}

	if (Character->HasAuthority())
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("TimerCallback"), Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SpawnDelay, false);
	}
}

void UMcAction_ProjectileCast::TimerCallback(ACharacter* Character)
{
	const FTransform SpawnTM = GetProjectileSpawnTM(Character);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Character;

	if (ensure(ProjectileClass))
	{
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(Character);
}

FTransform UMcAction_ProjectileCast::GetProjectileSpawnTM(const ACharacter* Character) const
{
	FRotator ControlRotation = Character->GetControlRotation();
	FVector HandLocation = Character->GetMesh()->GetSocketLocation(SocketName);
	FVector CameraLocation = Character->GetPawnViewLocation();

	FVector TraceEnd = CameraLocation + (ControlRotation.Vector() * MaxDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FHitResult Hit;

	bool bHitFound = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TraceEnd, ObjectQueryParams);

	FVector Target = bHitFound ? Hit.Location : TraceEnd;

	FVector ProjectilePath = Target - HandLocation;
	FRotator ProjectileRotation = ProjectilePath.Rotation();

	FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);

	return SpawnTM;
}
