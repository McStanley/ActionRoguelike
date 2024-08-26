// Fill out your copyright notice in the Description page of Project Settings.


#include "McActionComponent.h"

#include "McAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_McStanley);

UMcActionComponent::UMcActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UMcActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMcActionComponent, Actions);
}

void UMcActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UMcAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UMcActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<TObjectPtr<UMcAction>> ActionsCopy = Actions;

	for (TObjectPtr<UMcAction> Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}


void UMcActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// for (UMcAction* Action : Actions)
	// {
	// 	FColor TextColor = Action->IsRunning() ? FColor::Emerald : FColor::White;
	// 	FString Message = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	//
	// 	LogOnScreen(this, Message, TextColor, 0.f);
	// }
}

bool UMcActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UMcAction* Action : Actions)
	{
		if (Action)
		{
			bWroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UMcActionComponent::AddAction(AActor* Instigator, TSubclassOf<UMcAction> ActionClass)
{
	if (!ensure(ActionClass)) return;

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempted to AddAction. (ActionClass: %s)"), *GetNameSafe(ActionClass))
		return;
	}

	UMcAction* NewAction = NewObject<UMcAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UMcActionComponent::RemoveAction(UMcAction* Action)
{
	if (ensure(Action && !Action->IsRunning()))
	{
		Actions.Remove(Action);
	}
}

bool UMcActionComponent::HasAction(TSubclassOf<UMcAction> ActionClass)
{
	for (UMcAction* Action : Actions)
	{
		if (Action->ActionName == ActionClass.GetDefaultObject()->ActionName)
		{
			return true;
		}
	}

	return false;
}

bool UMcActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (UMcAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

void UMcActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

bool UMcActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (UMcAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UMcActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}
