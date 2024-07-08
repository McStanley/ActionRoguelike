// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "McGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "McItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AMcItemChest : public AActor, public IMcGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMcItemChest();

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;
};
