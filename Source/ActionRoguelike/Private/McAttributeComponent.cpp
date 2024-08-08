// Fill out your copyright notice in the Description page of Project Settings.


#include "McAttributeComponent.h"

#include "McGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("mc.DamageMultiplier"), 1.0f, TEXT("Global damage multiplier for Attribute Component."), ECVF_Cheat
);

// Sets default values for this component's properties
UMcAttributeComponent::UMcAttributeComponent()
{
	Health = 100;
	HealthMax = 100;

	Rage = 0;
	RageMax = 30;

	SetIsReplicatedByDefault(true);
}

UMcAttributeComponent* UMcAttributeComponent::GetAttributeComponent(AActor* FromActor)
{
	if (FromActor == nullptr)
	{
		return nullptr;
	}

	return FromActor->FindComponentByClass<UMcAttributeComponent>();
}

bool UMcAttributeComponent::IsActorAlive(AActor* Actor)
{
	UMcAttributeComponent* AttributeComp = GetAttributeComponent(Actor);
	if (AttributeComp == nullptr)
	{
		return false;
	}

	return AttributeComp->IsALive();
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

bool UMcAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta, const bool bReflected)
{
	if (Delta < 0.f)
	{
		if (!GetOwner()->CanBeDamaged())
		{
			return false;
		}

		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	const float OldHealth = Health;

	const float NewHealth = Health + Delta;
	Health = FMath::Clamp(NewHealth, 0, HealthMax);

	const float ActualDelta = Health - OldHealth;

	if (ActualDelta != 0.f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta, bReflected);
	}

	if (ActualDelta < 0.f)
	{
		ApplyRageChange(InstigatorActor, -ActualDelta);

		if (Health == 0.f)
		{
			AMcGameModeBase* GM = GetWorld()->GetAuthGameMode<AMcGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

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

	MulticastHealthChanged(InstigatorActor, Health, Delta, false);

	return true;
}

void UMcAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta,
                                                                  bool bReflected)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta, bReflected);
}

bool UMcAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

float UMcAttributeComponent::GetRage() const
{
	return Rage;
}

float UMcAttributeComponent::GetRagePercent() const
{
	return Rage / RageMax;
}

bool UMcAttributeComponent::HasFullRage() const
{
	return Rage == RageMax;
}

bool UMcAttributeComponent::ApplyRageChange(AActor* InstigatorActor, const float Damage)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Damage, 0, RageMax);

	if (Rage == OldRage)
	{
		return false;
	}

	const float Delta = Rage - OldRage;
	OnRageChanged.Broadcast(InstigatorActor, this, Rage, GetRagePercent(), Delta);

	return true;
}

void UMcAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMcAttributeComponent, Health);
	DOREPLIFETIME(UMcAttributeComponent, HealthMax);
}
