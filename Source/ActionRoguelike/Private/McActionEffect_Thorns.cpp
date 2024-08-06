// Fill out your copyright notice in the Description page of Project Settings.


#include "McActionEffect_Thorns.h"

#include "McActionComponent.h"
#include "McAttributeComponent.h"

UMcActionEffect_Thorns::UMcActionEffect_Thorns()
{
	ActionName = FName("Thorns");

	Duration = 0;
	Period = 0;

	bAutoStart = true;
}

bool UMcActionEffect_Thorns::CanStart_Implementation(AActor* Instigator)
{
	AActor* Owner = GetOwningComponent()->GetOwner();
	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(Owner);

	if (!AttributeComp)
	{
		return false;
	}

	return Super::CanStart_Implementation(Instigator);
}

void UMcActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(Owner);

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UMcActionEffect_Thorns::OnOwnerHealthChanged);
	}
}

void UMcActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(Owner);

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UMcActionEffect_Thorns::OnOwnerHealthChanged);
	}
}

void UMcActionEffect_Thorns::OnOwnerHealthChanged(AActor* InstigatorActor, UMcAttributeComponent* OwningComp,
                                                  float NewHealth, float Delta, bool bReflected)
{
	AActor* ActionOwner = GetOwningComponent()->GetOwner();

	const bool bSelfDamage = InstigatorActor == ActionOwner;

	if (!bReflected && Delta < 0.f && !bSelfDamage)
	{
		int32 DeltaFraction = FMath::RoundToInt32<float>(FMath::Min(Delta / 10.f, -1.f));

		UMcAttributeComponent* AttackerAttributeComp = UMcAttributeComponent::GetAttributeComponent(InstigatorActor);
		if (AttackerAttributeComp)
		{
			AttackerAttributeComp->ApplyHealthChange(ActionOwner, DeltaFraction, true);
		}
	}
}
