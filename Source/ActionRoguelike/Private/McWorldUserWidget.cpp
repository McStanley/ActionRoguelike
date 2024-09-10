// Fill out your copyright notice in the Description page of Project Settings.


#include "McWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void UMcWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AnchorActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AnchorActor not valid, removing widget."));
		return;
	}

	const FVector WorldLocation = AnchorActor->GetActorLocation() + WorldOffset;
	FVector2D ScreenPosition;

	const bool bIsOnScreen = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetOwningPlayer(), WorldLocation, ScreenPosition, false);

	if (ParentSizeBox)
	{
		if (bIsOnScreen)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}

		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}
