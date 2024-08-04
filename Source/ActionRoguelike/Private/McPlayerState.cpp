// Fill out your copyright notice in the Description page of Project Settings.


#include "McPlayerState.h"

AMcPlayerState::AMcPlayerState()
{
	Credits = 0;
}

int32 AMcPlayerState::GetCredits() const
{
	return Credits;
}

bool AMcPlayerState::AddCredits(AActor* InstigatorActor, int32 Amount)
{
	if (!ensure(Amount > 0))
	{
		return false;
	}

	Credits += Amount;

	OnCreditsChanged.Broadcast(InstigatorActor, this, Credits, Amount);

	return true;
}

bool AMcPlayerState::SpendCredits(AActor* InstigatorActor, int32 Amount)
{
	if (!ensure(Amount > 0))
	{
		return false;
	}

	if (Amount > Credits)
	{
		return false;
	}

	Credits -= Amount;

	OnCreditsChanged.Broadcast(InstigatorActor, this, Credits, -Amount);

	return true;
}
