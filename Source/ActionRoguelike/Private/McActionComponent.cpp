// Fill out your copyright notice in the Description page of Project Settings.


#include "McActionComponent.h"

#include "McAction.h"

UMcActionComponent::UMcActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMcActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UMcAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


void UMcActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMcActionComponent::AddAction(AActor* Instigator, TSubclassOf<UMcAction> ActionClass)
{
	if (!ensure(ActionClass)) return;

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

bool UMcActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (UMcAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				continue;
			}

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UMcActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (UMcAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}
