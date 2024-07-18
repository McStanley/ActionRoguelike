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

bool UMcAttributeComponent::ApplyHealthChange(const float Delta)
{
	const float NewHealth = Health + Delta;
	Health = FMath::Clamp(NewHealth, 0, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}
