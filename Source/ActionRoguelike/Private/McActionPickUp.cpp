// Fill out your copyright notice in the Description page of Project Settings.


#include "McActionPickUp.h"

#include "McActionComponent.h"

void AMcActionPickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	UMcActionComponent* ActionComp = InstigatorPawn->GetComponentByClass<UMcActionComponent>();
	if (ActionComp)
	{
		if (!ActionComp->HasAction(ActionClass))
		{
			ActionComp->AddAction(this, ActionClass);
			Deactivate();
		}
	}
}
