// Fill out your copyright notice in the Description page of Project Settings.


#include "McCoin.h"

#include "McPlayerState.h"

AMcCoin::AMcCoin()
{
	RespawnDelay = 5.0f;
	CreditsAward = 2;
}

void AMcCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	AMcPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AMcPlayerState>();
	if (PlayerState)
	{
		PlayerState->AddCredits(this, CreditsAward);

		Deactivate();
	}
}
