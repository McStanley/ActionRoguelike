// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McAICharacter.h"

#include "AIController.h"
#include "McAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AMcAICharacter::AMcAICharacter()
{
	AttributeComp = CreateDefaultSubobject<UMcAttributeComponent>("AttributeComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void AMcAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMcAICharacter::OnPawnSeen);
}

void AMcAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Player spotted", nullptr, FColor::White, 4.f, true);
	}
}
