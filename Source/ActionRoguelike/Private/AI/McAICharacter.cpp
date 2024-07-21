// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McAICharacter.h"

// Sets default values
AMcAICharacter::AMcAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMcAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMcAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
