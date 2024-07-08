// Fill out your copyright notice in the Description page of Project Settings.


#include "McItemChest.h"

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
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}
