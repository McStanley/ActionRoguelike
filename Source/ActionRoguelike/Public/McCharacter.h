// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "McCharacter.generated.h"

class UMcInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AMcCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	UAnimMontage* MagicProjectileAnim;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	UAnimMontage* BlackHoleAnim;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor> TeleportClass;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	UAnimMontage* TeleportAnim;

	FTimerHandle TimerHandle_Projectile;
	FTimerDelegate TimerDelegate_Projectile;

public:
	// Sets default values for this character's properties
	AMcCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UMcInteractionComponent* InteractionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Teleport();
	void PrimaryAttack();
	void SecondaryAttack();
	void PrimaryInteract();

	void UseProjectile(UAnimMontage* AnimMontage,
	                   const float TimerDelay,
	                   TSubclassOf<AActor>& Class,
	                   FName Socket,
	                   float MaxDistance = 10000.f);

	UFUNCTION()
	void UseProjectile_TimerCallback(const TSubclassOf<AActor>& Class, FName Socket, float MaxDistance);

	FTransform GetProjectileSpawnTM(const FName Socket, float MaxDistance) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
