// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McBTTask_HealSelf.h"

#include "AIController.h"
#include "McAttributeComponent.h"

EBTNodeResult::Type UMcBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) return EBTNodeResult::Failed;

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr) return EBTNodeResult::Failed;

	UMcAttributeComponent* AttributeComp = Cast<UMcAttributeComponent>(
		AIPawn->GetComponentByClass(UMcAttributeComponent::StaticClass())
	);
	if (AttributeComp == nullptr) return EBTNodeResult::Failed;

	const bool HealingSuccess = AttributeComp->SetHealthToMax(AIPawn);

	return HealingSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
