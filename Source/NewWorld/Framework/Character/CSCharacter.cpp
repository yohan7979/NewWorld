// Fill out your copyright notice in the Description page of Project Settings.


#include "NewWorld/Framework/Character/CSCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/BlendSpace.h"
#include "NewWorld/Framework/Weapon/CSWeapon.h"
#include "NewWorld/Framework/CSPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "NewWorld/Interface/CSInteractionInterface.h"
#include "NewWorld/Framework/Component/CSAttributeComponent.h"


FOnTakeDamageEvent ACSCharacter::OnTakeDamageEvent;

ACSCharacter::ACSCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->TargetArmLength = 400.f;
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

	AttributeComponent = ObjectInitializer.CreateDefaultSubobject<UCSAttributeComponent>(this, TEXT("AttributeComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

// Called when the game starts or when spawned
void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PressedJumpEvent.AddUObject(this, &ACSCharacter::DoRoll);
}

void ACSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearAllTimersForObject(this);
	TimerManager.ClearTimer(TimerHandle_OrientRotationMode);
	TimerManager.ClearTimer(TimerHandle_RollEnd);

	PressedJumpEvent.RemoveAll(this);
}

void ACSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetLocalRole() == ROLE_Authority)
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

	if (GetLocalRole() == ROLE_Authority)
	{
		DestoryWeaponInventory();
	}
}

float ACSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (ResultDamage > 0.f)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			ReplicateHit(ResultDamage, DamageEvent, EventInstigator, DamageCauser, !IsAliveAndWell());
		}
	}

	return ResultDamage;
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
// 		bool bLimitRotation = GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling();
// 		const FRotator& Rotation = bLimitRotation ? GetActorRotation() : GetControlRotation();
// 		const FVector& Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
// 		AddMovementInput(Direction, Value);

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACSCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
// 		const FQuat& Rotation = GetActorQuat();
// 		const FVector& Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
// 		AddMovementInput(Direction, Value);

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
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
	if (PressedJumpEvent.IsBound())
	{
		PressedJumpEvent.Broadcast(bPressed);
	}
	else
	{
		bPressed ? Jump() : StopJumping();
	}
}

void ACSCharacter::DoRoll(bool bPressed)
{
	if (bPressed && CanRoll())
	{
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerStartRoll();
		}

		auto RollStateLambda = [this](bool bInRollState) {
			SetRollState(bInRollState);
		};

		RollStateLambda(true);

		if (RollAnim)
		{
			const float DesiredPlayTime = PlayAnimMontage(RollAnim) * (1.f / RollAnim->RateScale);

			FTimerManager& TimerManager = GetWorldTimerManager();
			if (TimerHandle_RollEnd.IsValid())
			{
				TimerManager.ClearTimer(TimerHandle_RollEnd);
			}

			TimerManager.SetTimer(TimerHandle_RollEnd, FTimerDelegate::CreateLambda(RollStateLambda, false), DesiredPlayTime, false);
		}
	}
}

void ACSCharacter::SetRollState(bool bRoll)
{
	ACSPlayerController* PC = Cast<ACSPlayerController>(GetController());
	if (PC && PC->IsLocalController())
	{
		PC->SetIgnoreFireInput(bRoll);

		if (bRoll && Weapon)
		{
			Weapon->StopFire(0);
		}
	}

	bIsRolling = bRoll;
}

bool ACSCharacter::CanRoll() const
{
	return GetCharacterMovement()->IsMovingOnGround() && !bIsRolling;
}

void ACSCharacter::OnRep_IsRolling()
{
	if (bIsRolling)
	{
		PlayAnimMontage(RollAnim);
	}
}

void ACSCharacter::ServerStartRoll_Implementation()
{
	DoRoll(true);
}

bool ACSCharacter::ServerStartRoll_Validate()
{
	return true;
}

void ACSCharacter::SetOrientRotationMode(bool bOrientRotationToMovement, float DelayTime /*= 0.f*/)
{
	auto SetOrientModeLambda = [this](bool bEnable)
	{
		GetCharacterMovement()->bOrientRotationToMovement = bEnable;
		GetCharacterMovement()->bUseControllerDesiredRotation = !bEnable;
	};

	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerHandle_OrientRotationMode.IsValid())
	{
		TimerManager.ClearTimer(TimerHandle_OrientRotationMode);
	}

	if (DelayTime > 0.f)
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda(SetOrientModeLambda, bOrientRotationToMovement);
		TimerManager.SetTimer(TimerHandle_OrientRotationMode, TimerDelegate, DelayTime, false);	
	}
	else
	{
		SetOrientModeLambda(bOrientRotationToMovement);
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
	if (GetLocalRole() < ROLE_Authority)
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
	if (GetLocalRole() < ROLE_Authority)
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
	if (GetLocalRole() == ROLE_Authority)
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

void ACSCharacter::SetCharacterStatus(const FCharacterStatus& InStatus)
{
	if (IsValid(AttributeComponent))
	{
		AttributeComponent->CharacterStatus = InStatus;
	}
}

void ACSCharacter::OnRep_Weapon(ACSWeapon* LastWeapon)
{
	// Assign Weapon on client
	SetCurrentWeapon(Weapon, LastWeapon);
}

void ACSCharacter::OnRep_LastTakeHitInfo()
{
	if (LastTakeHitInfo.bKilled)
	{
		OnDeath(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
	else
	{
		PlayHit(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
}


void ACSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSCharacter, Weapon);
	DOREPLIFETIME(ACSCharacter, LastTakeHitInfo);
	DOREPLIFETIME_CONDITION(ACSCharacter, WeaponInventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACSCharacter, bIsRolling, COND_SkipOwner);
}

bool ACSCharacter::IsAliveAndWell() const
{
	return !IsPendingKill() && IsValid(AttributeComponent) && AttributeComponent->Health > 0.f;
}

void ACSCharacter::ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser, bool bKilled)
{
	LastTakeHitInfo.ActualDamage = Damage;
	LastTakeHitInfo.PawnInstigator = EventInstigator ? Cast<ACSCharacter>(EventInstigator->GetPawn()) : nullptr;
	LastTakeHitInfo.DamageCauser = DamageCauser;
	LastTakeHitInfo.bKilled = bKilled;
	LastTakeHitInfo.SetDamageEvent(DamageEvent);
	LastTakeHitInfo.EnsureReplication();

	OnRep_LastTakeHitInfo();
}

void ACSCharacter::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class ACSCharacter* PawnInstigator, class AActor* DamageCauser)
{

}

void ACSCharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class ACSCharacter* PawnInstigator, class AActor* DamageCauser)
{
	if (ACSCharacter::OnTakeDamageEvent.IsBound())
	{
		ACSCharacter::OnTakeDamageEvent.Broadcast(this, DamageTaken, DamageEvent, PawnInstigator ? PawnInstigator->GetController() : nullptr, DamageCauser);
	}
}

float ACSCharacter::SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewTransform)
{
	if (nullptr == ObjectInfo)
	{
		return 0.f;
	}

	return GetSignificanceByDistance(ObjectInfo->GetObject(), ViewTransform);
}

void ACSCharacter::PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
{

}

FName ACSCharacter::GetObjectTag() const
{
	return TEXT("Character");
}
