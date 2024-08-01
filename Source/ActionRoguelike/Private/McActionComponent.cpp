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
}


void UMcActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMcActionComponent::AddAction(TSubclassOf<UMcAction> ActionClass)
{
	if (!ensure(ActionClass)) return;

	UMcAction* NewAction = NewObject<UMcAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UMcActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (UMcAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}
