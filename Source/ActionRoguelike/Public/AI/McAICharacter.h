// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "McAICharacter.generated.h"

class UMcActionComponent;
class UMcAttributeComponent;
class UPawnSensingComponent;
class UMcWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AMcAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMcAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMcAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMcActionComponent* ActionComp;

	UPROPERTY()
	UMcWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMcWorldUserWidget> SpottedWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMcAttributeComponent* OwningComp, float NewHealth, float Delta,
	                     bool bReflected);

	AActor* GetTargetActor();

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents() override;
};
