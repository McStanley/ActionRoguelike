// Fill out your copyright notice in the Description page of Project Settings.


#include "McCharacter.h"

#include "McActionComponent.h"
#include "McAttributeComponent.h"
#include "McInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMcCharacter::AMcCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	AttributeComp = CreateDefaultSubobject<UMcAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UMcActionComponent>("ActionComp");

	InteractionComp = CreateDefaultSubobject<UMcInteractionComponent>("InteractionComp");

	TimeToHitParamName = "TimeToHit";

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void AMcCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AMcCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void AMcCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMcCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMcCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMcCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMcCharacter::SprintStop);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &AMcCharacter::Teleport);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AMcCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AMcCharacter::SecondaryAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AMcCharacter::PrimaryInteract);
}

void AMcCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void AMcCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// ControlRot.Yaw += 90.0f;
	// AddMovementInput(ControlRot.Vector(), Value);

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AMcCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AMcCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AMcCharacter::Teleport()
{
	ActionComp->StartActionByName(this, "Teleport");
}

void AMcCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AMcCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void AMcCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void AMcCharacter::OnHealthChanged(AActor* InstigatorActor, UMcAttributeComponent* OwningComp, float NewHealth,
                                   float Delta, bool bReflected)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());

		if (NewHealth <= 0.f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());

			DisableInput(PC);
		}
	}
}

void AMcCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

FVector AMcCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}
