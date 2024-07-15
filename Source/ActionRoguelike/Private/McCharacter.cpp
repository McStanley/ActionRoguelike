// Fill out your copyright notice in the Description page of Project Settings.


#include "McCharacter.h"

#include "McAttributeComponent.h"
#include "McInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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

	InteractionComp = CreateDefaultSubobject<UMcInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
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

void AMcCharacter::Teleport()
{
	UseProjectile(TeleportAnim, 0.2f, TeleportClass, "Muzzle_02", 1000.f);
}

void AMcCharacter::PrimaryAttack()
{
	UseProjectile(MagicProjectileAnim, 0.2f, MagicProjectileClass, "Muzzle_01");
}

void AMcCharacter::SecondaryAttack()
{
	UseProjectile(BlackHoleAnim, 0.15f, BlackHoleClass, "Muzzle_01");
}

void AMcCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void AMcCharacter::UseProjectile(UAnimMontage* AnimMontage,
                                 const float TimerDelay,
                                 TSubclassOf<AActor>& Class,
                                 FName Socket,
                                 float MaxDistance)
{
	if (ensureAlways(AnimMontage && Class))
	{
		PlayAnimMontage(AnimMontage);

		TimerDelegate_Projectile.BindUFunction(this, FName("UseProjectile_TimerCallback"), Class, Socket, MaxDistance);

		GetWorldTimerManager().SetTimer(TimerHandle_Projectile, TimerDelegate_Projectile, TimerDelay, false);
	}
}

void AMcCharacter::UseProjectile_TimerCallback(const TSubclassOf<AActor>& Class, FName Socket, float MaxDistance)
{
	const FTransform SpawnTM = GetProjectileSpawnTM(Socket, MaxDistance);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(Class, SpawnTM, SpawnParams);
}

FTransform AMcCharacter::GetProjectileSpawnTM(const FName Socket, float MaxDistance) const
{
	FRotator ControlRotation = this->GetControlRotation();
	FVector HandLocation = GetMesh()->GetSocketLocation(Socket);
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	FVector TraceEnd = CameraLocation + (ControlRotation.Vector() * MaxDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FHitResult Hit;

	bool bHitFound = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TraceEnd, ObjectQueryParams);

	FVector Target = bHitFound ? Hit.Location : TraceEnd;
	// DrawDebugString(GetWorld(), Target, "Target here", nullptr, FColor::Blue, 5.f, true);

	FVector ProjectilePath = Target - HandLocation;
	FRotator ProjectileRotation = ProjectilePath.Rotation();

	FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);

	return SpawnTM;
}
