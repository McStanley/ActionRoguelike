// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "McWorldUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UMcWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AnchorActor;

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
