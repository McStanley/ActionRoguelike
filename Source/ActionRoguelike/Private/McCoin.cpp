// Fill out your copyright notice in the Description page of Project Settings.


#include "McCoin.h"

#include "McPlayerState.h"

AMcCoin::AMcCoin()
{
	RespawnDelay = 5.0f;
}

void AMcCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	AMcPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AMcPlayerState>();
	if (PlayerState)
	{
		PlayerState->AddCredits(this, 2);

		Deactivate();
	}
}
