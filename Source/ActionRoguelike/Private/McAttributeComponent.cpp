// Fill out your copyright notice in the Description page of Project Settings.


#include "McAttributeComponent.h"

// Sets default values for this component's properties
UMcAttributeComponent::UMcAttributeComponent()
{
	Health = 100;
}

bool UMcAttributeComponent::ApplyHealthChange(const float Delta)
{
	Health += Delta;

	return true;
}
