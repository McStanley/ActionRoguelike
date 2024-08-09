// Fill out your copyright notice in the Description page of Project Settings.


#include "McItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMcItemChest::AMcItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110.0;

	bReplicates = true;
}

void AMcItemChest::OnActorStateLoaded_Implementation()
{
	OnRep_LidOpened();
}

void AMcItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMcItemChest, bLidOpened);
}

// Called when the game starts or when spawned
void AMcItemChest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMcItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMcItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void AMcItemChest::OnRep_LidOpened()
{
	float NewPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(NewPitch, 0, 0));
}
