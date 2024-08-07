// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "McInteractionComponent.generated.h"


class UMcWorldUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UMcInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMcInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindBestTarget();

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InTarget);

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMcWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UMcWorldUserWidget* DefaultWidgetInstance;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime,
	                           ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();
};
