// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACSCharacter::ACSCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);

	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

// Called when the game starts or when spawned
void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACSCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACSCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnAt"), this, &ACSCharacter::TurnAt);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACSCharacter::LookUp);
}

void ACSCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		bool bLimitRotation = GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling();
		const FRotator& Rotation = bLimitRotation ? GetActorRotation() : GetControlRotation();
		const FVector& Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACSCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		const FQuat& Rotation = GetActorQuat();
		const FVector& Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACSCharacter::TurnAt(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACSCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

