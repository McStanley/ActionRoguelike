// Fill out your copyright notice in the Description page of Project Settings.


#include "McInteractionComponent.h"

#include "McGameplayInterface.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(
	TEXT("mc.DrawDebugInteraction"), false, TEXT("Enable DrawDebug helpers for Interaction Component."), ECVF_Cheat
);

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
	const bool bDrawDebugHelpers = CVarDrawDebugInteraction.GetValueOnGameThread();

	TArray<FHitResult> Hits;

	FVector EyeLocation;
	FRotator EyeRotation;

	AActor* Owner = GetOwner();
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	if (bDrawDebugHelpers)
	{
		DrawDebugString(GetWorld(), CameraLocation, "Camera here", nullptr, FColor::Blue, 5.f, true);
	}

	FVector End = CameraLocation + (EyeRotation.Vector() * 1000);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	constexpr float Radius = 30.f;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	const bool bHitFound = GetWorld()->SweepMultiByObjectType(Hits, CameraLocation, End, FQuat::Identity,
	                                                          ObjectQueryParams, Shape);

	const FColor DebugColor = bHitFound ? FColor::Green : FColor::Red;
	constexpr float DebugLifeTime = 2.0f;

	bool bInteractExecuted = false;

	for (FHitResult Hit : Hits)
	{
		if (bDrawDebugHelpers)
		{
			DrawDebugSphere(GetWorld(), Hit.Location, Radius, 32, DebugColor, false, DebugLifeTime);
		}

		AActor* HitActor = Hit.GetActor();

		if (HitActor && HitActor->Implements<UMcGameplayInterface>())
		{
			IMcGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(Owner));

			if (bDrawDebugHelpers)
			{
				DrawDebugLine(GetWorld(), EyeLocation, Hit.Location, DebugColor, false, DebugLifeTime, 0, 2.0f);
			}

			bInteractExecuted = true;
			break;
		}
	}

	if (!bInteractExecuted)
	{
		if (bDrawDebugHelpers)
		{
			DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, DebugLifeTime, 0, 2.0f);
		}
	}
}
