// Fill out your copyright notice in the Description page of Project Settings.


#include "McHealthPotion.h"

#include "McAttributeComponent.h"

AMcHealthPotion::AMcHealthPotion()
{
	RespawnDelay = 2.5f;
}

void AMcHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	// DrawDebugString(GetWorld(), GetActorLocation(), "Interact");

	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(InstigatorPawn);

	if (AttributeComp != nullptr)
	{
		if (!AttributeComp->HasFullHealth())
		{
			AttributeComp->SetHealthToMax(this);

			Deactivate();
		}
	}
}
