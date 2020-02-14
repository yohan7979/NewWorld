// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/BlendSpace.h"
#include "Net/UnrealNetwork.h"
#include "CSWeapon.h"

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

void ACSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		SpawnDefaultInventory();
	}
}

// Called every frame
void ACSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSCharacter::Destroyed()
{
	Super::Destroyed();

	if (Role == ROLE_Authority)
	{
		DestoryInventory();
	}
}

// Called to bind functionality to input
void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACSCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACSCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnAt"), this, &ACSCharacter::TurnAt);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACSCharacter::LookUp);

	PlayerInputComponent->BindAction<FBindIntegerDelegate>(TEXT("SwitchWeaponOne"), IE_Pressed, this, &ACSCharacter::SwitchWeapon, 1);
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

void ACSCharacter::SwitchWeapon(int32 index)
{
	if (Inventory.IsValidIndex(index))
	{
		ACSWeapon* DesiredWeapon = Inventory[index];
		if (IsValid(DesiredWeapon))
		{
			EquipWeapon(DesiredWeapon);
		}
	}
}

void ACSCharacter::SpawnDefaultInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	for (auto& WeaponClass : DefaultInventoryClasses)
	{
		if (IsValid(WeaponClass))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ACSWeapon* NewWeapon = GetWorld()->SpawnActor<ACSWeapon>(WeaponClass.Get(), SpawnParams);
			AddWeapon(NewWeapon);
		}
	}

	// 첫 번째 무기 자동 장착
	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void ACSCharacter::DestoryInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	for (ACSWeapon* TargetWeapon : Inventory)
	{
		if (TargetWeapon)
		{
			RemoveWeapon(TargetWeapon);
			TargetWeapon->Destroy();
		}
	}
}

void ACSCharacter::AddWeapon(ACSWeapon* InWeapon)
{
	if (IsValid(InWeapon))
	{
		InWeapon->OnEnterInventory(this);
		Inventory.AddUnique(InWeapon);
	}
}

void ACSCharacter::RemoveWeapon(ACSWeapon* InWeapon)
{
	if (IsValid(InWeapon))
	{
		InWeapon->OnLeaveInventory();
		Inventory.RemoveSingle(InWeapon);
	}
}

void ACSCharacter::ServerEquipWeapon_Implementation(ACSWeapon* NewWeapon)
{
	EquipWeapon(NewWeapon);
}

bool ACSCharacter::ServerEquipWeapon_Validate(ACSWeapon* NewWeapon)
{
	return true;
}

void ACSCharacter::EquipWeapon(ACSWeapon* NewWeapon)
{
	if (NewWeapon)
	{
		if (Role == ROLE_Authority)
		{
			SetCurrentWeapon(NewWeapon, Weapon);
		}
		else
		{
			ServerEquipWeapon(NewWeapon);
		}
	}
}

void ACSCharacter::SetCurrentWeapon(ACSWeapon* NewWeapon, ACSWeapon* LastWeapon)
{
	ACSWeapon* LocalLastWeapon = nullptr;
	
	if (LastWeapon)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != Weapon)
	{
		LocalLastWeapon = Weapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	// Set New Weapon
	if (Weapon != NewWeapon)
	{
		Weapon = NewWeapon;
	}
	
	if (Weapon)
	{
		Weapon->OnEquip();
	}
}

void ACSCharacter::OnRep_Weapon(ACSWeapon* LastWeapon)
{
	// Assign Weapon on client
	SetCurrentWeapon(Weapon, LastWeapon);
}

void ACSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSCharacter, Weapon);
	DOREPLIFETIME_CONDITION(ACSCharacter, Inventory, COND_OwnerOnly);
}

