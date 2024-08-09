// Fill out your copyright notice in the Description page of Project Settings.


#include "McPlayerState.h"

#include "McSaveGame.h"
#include "Net/UnrealNetwork.h"

AMcPlayerState::AMcPlayerState()
{
	Credits = 0;
}

void AMcPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMcPlayerState, Credits);
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

	ClientCreditsChanged(InstigatorActor, Credits, Amount);

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

	ClientCreditsChanged(InstigatorActor, Credits, -Amount);

	return true;
}

void AMcPlayerState::ClientCreditsChanged_Implementation(AActor* InstigatorActor, int32 NewCredits, int32 Delta)
{
	OnCreditsChanged.Broadcast(InstigatorActor, this, NewCredits, Delta);
}

void AMcPlayerState::Save_Implementation(UMcSaveGame* SaveGame)
{
	if (SaveGame)
	{
		SaveGame->Credits = Credits;
	}
}

void AMcPlayerState::Load_Implementation(UMcSaveGame* SaveGame)
{
	if (SaveGame)
	{
		// Credits = SaveGame->Credits;
		AddCredits(nullptr, SaveGame->Credits);
	}
}
