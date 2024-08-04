// Fill out your copyright notice in the Description page of Project Settings.


#include "McInteractionComponent.h"

#include "McGameplayInterface.h"
#include "McWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(
	TEXT("mc.DrawDebugInteraction"), false, TEXT("Enable DrawDebug helpers for Interaction Component."), ECVF_Cheat
);

UMcInteractionComponent::UMcInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.f;
	TraceRadius = 30.f;
	CollisionChannel = ECC_WorldDynamic;
}


void UMcInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMcInteractionComponent::TickComponent(float DeltaTime,
                                            ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestTarget();
}

void UMcInteractionComponent::FindBestTarget()
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

	FVector End = CameraLocation + (EyeRotation.Vector() * TraceDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	const bool bHitFound = GetWorld()->SweepMultiByObjectType(Hits, CameraLocation, End, FQuat::Identity,
	                                                          ObjectQueryParams, Shape);

	const FColor DebugColor = bHitFound ? FColor::Green : FColor::Red;
	constexpr float DebugLifeTime = 2.0f;

	TargetActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDrawDebugHelpers)
		{
			DrawDebugSphere(GetWorld(), Hit.Location, TraceRadius, 32, DebugColor, false, DebugLifeTime);
		}

		AActor* HitActor = Hit.GetActor();

		if (HitActor && HitActor->Implements<UMcGameplayInterface>())
		{
			TargetActor = HitActor;

			if (bDrawDebugHelpers)
			{
				DrawDebugLine(GetWorld(), EyeLocation, Hit.Location, DebugColor, false, DebugLifeTime, 0, 2.0f);
			}

			break;
		}
	}

	if (TargetActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UMcWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AnchorActor = TargetActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}

		if (bDrawDebugHelpers)
		{
			DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, DebugLifeTime, 0, 2.0f);
		}
	}
}

void UMcInteractionComponent::PrimaryInteract()
{
	if (TargetActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No TargetActor to interact with.");
		return;
	}

	IMcGameplayInterface::Execute_Interact(TargetActor, Cast<APawn>(GetOwner()));
}
