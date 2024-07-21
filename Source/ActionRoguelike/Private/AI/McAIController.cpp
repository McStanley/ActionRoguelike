// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McAIController.h"

void AMcAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr. Assign BahaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
