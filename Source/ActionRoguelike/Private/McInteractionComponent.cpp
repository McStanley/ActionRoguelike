// Fill out your copyright notice in the Description page of Project Settings.


#include "McInteractionComponent.h"

#include "McGameplayInterface.h"

// Sets default values for this component's properties
UMcInteractionComponent::UMcInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMcInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UMcInteractionComponent::TickComponent(float DeltaTime,
                                            ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMcInteractionComponent::PrimaryInteract()
{
	TArray<FHitResult> Hits;

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* Owner = GetOwner();
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	constexpr float Radius = 30.f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bHitFound = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams,
	                                                    Shape);

	const FColor DebugColor = bHitFound ? FColor::Green : FColor::Red;
	constexpr float DebugLifeTime = 2.0f;

	for (FHitResult Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.Location, Radius, 32, DebugColor, false, DebugLifeTime);

		AActor* HitActor = Hit.GetActor();

		if (HitActor && HitActor->Implements<UMcGameplayInterface>())
		{
			IMcGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(Owner));

			break;
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, DebugLifeTime, 0, 2.0f);
}
