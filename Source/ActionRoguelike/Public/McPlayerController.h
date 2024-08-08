// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "McPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
};
