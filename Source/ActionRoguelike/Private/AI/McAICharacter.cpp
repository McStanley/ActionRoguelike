// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "McActionComponent.h"
#include "McAttributeComponent.h"
#include "McWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AMcAICharacter::AMcAICharacter()
{
	AttributeComp = CreateDefaultSubobject<UMcAttributeComponent>("AttributeComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	ActionComp = CreateDefaultSubobject<UMcActionComponent>("ActionComp");

	TimeToHitParamName = "TimeToHit";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMcAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMcAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AMcAICharacter::OnHealthChanged);
}

void AMcAICharacter::OnPawnSeen(APawn* Pawn)
{
	AActor* CurrentTarget = GetTargetActor();
	if (CurrentTarget != Pawn)
	{
		SetTargetActor(Pawn);

		MulticastCreateSpottedWidget();
	}
}

void AMcAICharacter::OnHealthChanged(AActor* InstigatorActor, UMcAttributeComponent* OwningComp, float NewHealth,
                                     float Delta, bool bReflected)
{
	if (Delta < 0.f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UMcWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AnchorActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());

		if (NewHealth <= 0.f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.f);
		}
	}
}

AActor* AMcAICharacter::GetTargetActor()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		return TargetActor;
	}

	return nullptr;
}

void AMcAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AMcAICharacter::MulticastCreateSpottedWidget_Implementation()
{
	UMcWorldUserWidget* SpottedWidget = CreateWidget<UMcWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (SpottedWidget)
	{
		SpottedWidget->AnchorActor = this;
		SpottedWidget->WorldOffset = FVector(0, 0, 160.f);
		SpottedWidget->AddToViewport(1);
	}
}
