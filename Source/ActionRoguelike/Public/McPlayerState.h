// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "McPlayerState.generated.h"

class UMcSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreditsChanged, AActor*, InstigatorActor, AMcPlayerState*, OwningState,
                                              int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AMcPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMcPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

private:
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "State")
	int32 Credits;

public:
	UFUNCTION(BlueprintCallable, Category = "State")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool AddCredits(AActor* InstigatorActor, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "State")
	bool SpendCredits(AActor* InstigatorActor, int32 Amount);

	UFUNCTION(Client, Reliable)
	void ClientCreditsChanged(AActor* InstigatorActor, int32 NewCredits, int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void Save(UMcSaveGame* SaveGame);

	UFUNCTION(BlueprintNativeEvent)
	void Load(UMcSaveGame* SaveGame);
};
