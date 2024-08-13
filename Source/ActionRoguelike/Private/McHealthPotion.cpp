// Fill out your copyright notice in the Description page of Project Settings.


#include "McHealthPotion.h"

#include "McAttributeComponent.h"
#include "McPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

AMcHealthPotion::AMcHealthPotion()
{
	RespawnDelay = 2.5f;
	CreditsCost = 1;
}

FText AMcHealthPotion::GetInteractMessage_Implementation(APawn* InstigatorPawn)
{
	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(InstigatorPawn);

	if (AttributeComp && AttributeComp->HasFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(
		LOCTEXT("HealthPotion_InteractMessage", "Cost: {0} credits. Restores health to max."),
		CreditsCost
	);
}

void AMcHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	// DrawDebugString(GetWorld(), GetActorLocation(), "Interact");

	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(InstigatorPawn);

	if (AttributeComp && !AttributeComp->HasFullHealth())
	{
		if (AMcPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AMcPlayerState>())
		{
			if (PlayerState->SpendCredits(this, CreditsCost))
			{
				AttributeComp->SetHealthToMax(this);

				Deactivate();
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
