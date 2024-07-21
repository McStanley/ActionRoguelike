// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/McAIController.h"

void AMcAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}
