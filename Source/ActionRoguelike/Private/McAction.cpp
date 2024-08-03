// Fill out your copyright notice in the Description page of Project Settings.


#include "McAction.h"

#include "McActionComponent.h"

bool UMcAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	if (GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UMcAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Starting: %s"), *GetNameSafe(this));

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UMcAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning);

	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool UMcAction::IsRunning() const
{
	return bIsRunning;
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

UMcActionComponent* UMcAction::GetOwningComponent() const
{
	return Cast<UMcActionComponent>(GetOuter());
}
