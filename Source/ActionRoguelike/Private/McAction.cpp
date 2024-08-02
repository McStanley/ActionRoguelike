// Fill out your copyright notice in the Description page of Project Settings.


#include "McAction.h"

void UMcAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Starting: %s"), *GetNameSafe(this));
}

void UMcAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
}

UWorld* UMcAction::GetWorld() const
{
	UActorComponent* Outer = Cast<UActorComponent>(GetOuter());
	if (Outer)
	{
		return Outer->GetWorld();
	}

	return nullptr;
}
