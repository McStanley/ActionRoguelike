// Fill out your copyright notice in the Description page of Project Settings.


#include "McAttributeComponent.h"

// Sets default values for this component's properties
UMcAttributeComponent::UMcAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}

bool UMcAttributeComponent::IsALive() const
{
	return Health > 0.f;
}

float UMcAttributeComponent::GetHealth() const
{
	return Health;
}

float UMcAttributeComponent::GetHealthPercent() const
{
	return Health / HealthMax;
}

bool UMcAttributeComponent::HasFullHealth() const
{
	return Health == HealthMax;
}

bool UMcAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
	const float OldHealth = Health;

	const float NewHealth = Health + Delta;
	Health = FMath::Clamp(NewHealth, 0, HealthMax);

	const float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool UMcAttributeComponent::SetHealthToMax(AActor* InstigatorActor)
{
	const float Delta = HealthMax - Health;

	if (Delta == 0)
	{
		return false;
	}

	Health = HealthMax;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

	return true;
}
