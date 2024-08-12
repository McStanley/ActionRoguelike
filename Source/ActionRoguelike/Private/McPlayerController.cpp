// Fill out your copyright notice in the Description page of Project Settings.


#include "McPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMcPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AMcPlayerController::TogglePauseMenu);
}

void AMcPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}

void AMcPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance)
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		PauseMenuInstance = CreateWidget(this, PauseMenuClass);
		if (ensure(PauseMenuInstance))
		{
			PauseMenuInstance->AddToViewport(100);

			SetInputMode(FInputModeUIOnly());
			bShowMouseCursor = true;
		}
	}
}
