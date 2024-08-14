// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "McAction.generated.h"

class UMcActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UMcAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(Replicated)
	float TimeStarted;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	UFUNCTION(BlueprintCallable, Category = "Action")
	UMcActionComponent* GetOwningComponent() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
