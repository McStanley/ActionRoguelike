// Fill out your copyright notice in the Description page of Project Settings.


#include "McGameplayFunctionLibrary.h"

#include "McAttributeComponent.h"

bool UMcGameplayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount)
{
	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(TargetActor);
	if (AttributeComp == nullptr) return false;

	return AttributeComp->ApplyHealthChange(InstigatorActor, -DamageAmount);
}

bool UMcGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* InstigatorActor, AActor* TargetActor,
                                                        float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(InstigatorActor, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}

	return false;
}
