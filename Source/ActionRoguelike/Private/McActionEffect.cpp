// Fill out your copyright notice in the Description page of Project Settings.


#include "McActionEffect.h"

#include "McActionComponent.h"
#include "GameFramework/GameStateBase.h"

UMcActionEffect::UMcActionEffect()
{
	bAutoStart = true;
}

void UMcActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UMcActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	UMcActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp)
	{
		ActionComp->RemoveAction(this);
	}
}

float UMcActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState();
	if (GS)
	{
		const float CurrentTime = GS->GetServerWorldTimeSeconds();
		const float EndTime = TimeStarted + Duration;

		return EndTime - CurrentTime;
	}

	return Duration;
}

float UMcActionEffect::GetTimePercentRemaining() const
{
	if (Duration == 0)
	{
		return 0.f;
	}

	return GetTimeRemaining() / Duration;
}

void UMcActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
