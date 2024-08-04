// Fill out your copyright notice in the Description page of Project Settings.


#include "McHealthPotion.h"

#include "McAttributeComponent.h"
#include "McPlayerState.h"

AMcHealthPotion::AMcHealthPotion()
{
	RespawnDelay = 2.5f;
	CreditsCost = 1;
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
