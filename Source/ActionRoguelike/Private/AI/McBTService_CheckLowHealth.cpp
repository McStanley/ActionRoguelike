// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McBTService_CheckLowHealth.h"

#include "AIController.h"
#include "McAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UMcBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;


	UMcAttributeComponent* AttributeComp = UMcAttributeComponent::GetAttributeComponent(AIPawn);
	if (!ensure(AttributeComp)) return;

	const float HealthPercent = AttributeComp->GetHealthPercent();

	const bool IsLowHealth = HealthPercent <= HealthThreshold;

	// if (IsLowHealth)
	// {
	// 	DrawDebugString(GetWorld(), AIPawn->GetActorLocation(), "Low health", nullptr, FColor::Red, 2.f, true);
	// }

	BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, IsLowHealth);
}
