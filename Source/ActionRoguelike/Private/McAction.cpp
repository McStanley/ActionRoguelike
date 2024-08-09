// Fill out your copyright notice in the Description page of Project Settings.


#include "McAction.h"

#include "McActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

bool UMcAction::IsSupportedForNetworking() const
{
	return true;
}

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
	// LogOnScreen(this, FString::Printf(TEXT("Starting: %s"), *ActionName.ToString()), FColor::Emerald);

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.Instigator = Instigator;
	RepData.bIsRunning = true;
}

void UMcAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Stopping: %s"), *ActionName.ToString()), FColor::Red);

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.Instigator = Instigator;
	RepData.bIsRunning = false;
}

bool UMcAction::IsRunning() const
{
	return RepData.bIsRunning;
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

void UMcAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

UMcActionComponent* UMcAction::GetOwningComponent() const
{
	return Cast<UMcActionComponent>(GetOuter());
}

void UMcAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMcAction, RepData);
}
