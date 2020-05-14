// Fill out your copyright notice in the Description page of Project Settings.


#include "CSCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/BlendSpace.h"
#include "CSWeapon.h"
#include "CSPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interface/CSInteractionInterface.h"

ACSCharacter::ACSCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);

	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	InteractionSphereComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("InteractionSphereComponent"));
	InteractionSphereComponent->SetSphereRadius(300.f);
	InteractionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphereComponent->SetCollisionObjectType(COLLISION_INTERACTIVE);
	InteractionSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	InteractionSphereComponent->SetGenerateOverlapEvents(true);
	InteractionSphereComponent->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);

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
		SpawnDefaultWeaponInventory();
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
		DestoryWeaponInventory();
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
	PlayerInputComponent->BindAction<FBindIntegerDelegate>(TEXT("SwitchWeaponTwo"), IE_Pressed, this, &ACSCharacter::SwitchWeapon, 2);
	PlayerInputComponent->BindAction<FBindBoolDelegate>(TEXT("DoJump"), IE_Pressed, this, &ACSCharacter::DoJump, true);
	PlayerInputComponent->BindAction<FBindBoolDelegate>(TEXT("DoJump"), IE_Released, this, &ACSCharacter::DoJump, false);

	PlayerInputComponent->BindAction(TEXT("OnStartFire"), IE_Pressed, this, &ACSCharacter::OnStartFire);
	PlayerInputComponent->BindAction(TEXT("OnStartFire"), IE_Released, this, &ACSCharacter::OnStopFire);

	PlayerInputComponent->BindAction(TEXT("OnInteract"), IE_Pressed, this, &ACSCharacter::OnInteract);
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

void ACSCharacter::DoJump(bool bPressed)
{
	if (bPressed)
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}

void ACSCharacter::OnStartFire()
{
	ACSPlayerController* Player = CastChecked<ACSPlayerController>(Controller);
	if (Player)
	{
		Player->StartFire(1);
	}
}


void ACSCharacter::OnStopFire()
{
	StopFire(1);
}

void ACSCharacter::PawnStartFire(uint8 FireModeNum /*= 0*/)
{
	if (IsValid(Weapon))
	{
		Weapon->StartFire(FireModeNum);
	}
}

void ACSCharacter::StopFire(const uint8 FireModeNum)
{
	if (IsValid(Weapon))
	{
		Weapon->StopFire(FireModeNum);
	}
}

void ACSCharacter::SwitchWeapon(int32 index)
{
	if (WeaponInventory.IsValidIndex(index - 1))
	{
		ACSWeapon* DesiredWeapon = WeaponInventory[index - 1];
		if (IsValid(DesiredWeapon))
		{
			EquipWeapon(DesiredWeapon);
		}
	}
}

void ACSCharacter::OnInteract()
{
	AActor* TargetActor = GetNearestInteractActor();
	if (TargetActor)
	{
		ServerInteract();
	}
}

void ACSCharacter::ServerInteract_Implementation()
{
	AActor* TargetActor = GetNearestInteractActor();
	if (TargetActor)
	{
		ICSInteractionInterface* InteractActor = Cast<ICSInteractionInterface>(TargetActor);
		if (InteractActor)
		{
			InteractActor->OnInteractWith(this);
		}
	}
}

bool ACSCharacter::ServerInteract_Validate()
{
	return true;
}

AActor* ACSCharacter::GetNearestInteractActor()
{
	AActor* NearestActor = nullptr;

	if (InteractionSphereComponent)
	{
		TArray<AActor*> OverlappedActors;
		InteractionSphereComponent->GetOverlappingActors(OverlappedActors);
		float NearestDistanceSquared = FLT_MAX;
		
		for (AActor* Actor : OverlappedActors)
		{
			if (IsValid(Actor))
			{
				ICSInteractionInterface* InteractActor = Cast<ICSInteractionInterface>(Actor);
				if (InteractActor && InteractActor->CanInteractWith(this))
				{
					float CurrentDistanceSq = FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation());
					if (NearestDistanceSquared > CurrentDistanceSq)
					{
						NearestDistanceSquared = CurrentDistanceSq;
						NearestActor = Actor;
					}
				}
			}
		}
	}

	return NearestActor;
}

void ACSCharacter::SpawnDefaultWeaponInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}
/*
	for (const auto& WeaponClass : DefaultWeaponInventoryClasses)
	{
		CreateAndGiveWeapon(WeaponClass);
	}

	// 첫 번째 무기 자동 장착
	if (WeaponInventory.Num() > 0)
	{
		EquipWeapon(WeaponInventory[0]);
	}
*/
}

void ACSCharacter::DestoryWeaponInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	for (ACSWeapon* TargetWeapon : WeaponInventory)
	{
		if (TargetWeapon)
		{
			RemoveWeapon(TargetWeapon);
			TargetWeapon->Destroy();
		}
	}
}

ACSWeapon* ACSCharacter::CreateAndGiveWeapon(const TSubclassOf<ACSWeapon>& WeaponClass)
{
	if (IsValid(WeaponClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACSWeapon* NewWeapon = GetWorld()->SpawnActor<ACSWeapon>(WeaponClass.Get(), SpawnParams);
		AddWeapon(NewWeapon);

		return NewWeapon;
	}

	return nullptr;
}

ACSWeapon* ACSCharacter::FindWeaponInventory(UClass* WeaponClass)
{
	ACSWeapon** FindWeapon = WeaponInventory.FindByPredicate([&](const ACSWeapon* Element)
	{
		return Element->GetClass() == WeaponClass;
	});

	if (FindWeapon)
	{
		return *FindWeapon;
	}

	return nullptr;
}

void ACSCharacter::AddWeapon(ACSWeapon* InWeapon)
{
	if (IsValid(InWeapon))
	{
		InWeapon->OnEnterWeaponInventory(this);
		WeaponInventory.AddUnique(InWeapon);
	}
}

void ACSCharacter::RemoveWeapon(ACSWeapon* InWeapon)
{
	if (IsValid(InWeapon))
	{
		InWeapon->OnLeaveWeaponInventory();
		WeaponInventory.RemoveSingle(InWeapon);
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
	if (Role == ROLE_Authority)
	{
		SetCurrentWeapon(NewWeapon, Weapon);
	}
	else
	{
		ServerEquipWeapon(NewWeapon);
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
		Weapon->SetOwningPawn(this);	// Make sure weapon's MyPawn is pointing back to us. During replication, we can't guarantee ACSCharacter::Weapon will rep after ACSWeapon::Owner!
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
	DOREPLIFETIME_CONDITION(ACSCharacter, WeaponInventory, COND_OwnerOnly);
}

