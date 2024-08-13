// Fill out your copyright notice in the Description page of Project Settings.


#include "McAnimInstance.h"

#include "GameplayTagContainer.h"
#include "McActionComponent.h"

void UMcAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = OwningActor->GetComponentByClass<UMcActionComponent>();
	}
}

void UMcAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
