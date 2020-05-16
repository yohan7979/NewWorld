// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "CSCharacter.h"
#include "CSWeaponState.h"
#include "CSWeaponState_Default.h"
#include "CSWeaponFiringAction.h"
#include "CSPlayerController.h"
#include "CSProjectile.h"
#include "Kismet/GameplayStatics.h"

ACSWeapon::ACSWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	SkelMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	SkelMeshComp->SetupAttachment(RootComponent);
	SkelMeshComp->SetGenerateOverlapEvents(false);
	SkelMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponStateClassMap.Emplace(EWeaponState::Active, UCSWeaponStateActive::StaticClass());
	WeaponStateClassMap.Emplace(EWeaponState::Inactive, UCSWeaponStateInactive::StaticClass());
	WeaponStateClassMap.Emplace(EWeaponState::Firing, UCSWeaponStateFiring::StaticClass());
	WeaponStateClassMap.Emplace(EWeaponState::Reloading, UCSWeaponStateReloading::StaticClass());
	WeaponStateClassMap.Emplace(EWeaponState::Equipping, UCSWeaponStateEquipping::StaticClass());

	SetRootComponent(SkelMeshComp);

	MuzzleSocketName = TEXT("MuzzleSocket");
}

// Called when the game starts or when spawned
void ACSWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (FiringAction)
	{
		FiringAction->Init(this);
	}
}

void ACSWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	WeaponStateClassMap.Empty();
	WeaponStateMap.Empty();

	if (FiringAction)
	{
		FiringAction->Shutdown();
	}
}

// Called every frame
void ACSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACSWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (FCSWeaponStateClassMap::TConstIterator It(WeaponStateClassMap); It; ++It)
	{
		UCSWeaponState* NewState = NewObject<UCSWeaponState>(this, It->Value.Get());
		WeaponStateMap.Emplace(It->Key, NewState);
	}

	CurrentStateNode = WeaponStateMap.FindRef(EWeaponState::Inactive);
}

void ACSWeapon::OnEquip()
{
	if (EquipTime < KINDA_SMALL_NUMBER)
	{
		AttachMeshToCharacter(true);
	}
	
	SetWeaponState(EWeaponState::Equipping);
}

void ACSWeapon::OnUnEquip()
{
	AttachMeshToCharacter(false);
		
	SetWeaponState(EWeaponState::Inactive);
}

void ACSWeapon::OnEnterWeaponInventory(ACSCharacter* NewOwner)
{
	AttachMeshToCharacter(false);

	SetOwningPawn(NewOwner);
}

void ACSWeapon::OnLeaveWeaponInventory()
{
	DetachMeshFromCharacter();

	SetOwningPawn(nullptr);
}

void ACSWeapon::AttachMeshToCharacter(bool bEquip)
{
	if (IsValid(CachedCharacter))
	{
		// Detach previous for safe
		DetachMeshFromCharacter();

		USkeletalMeshComponent* PawnMesh = CachedCharacter->GetMesh();
		const FName& AttachSocketName = bEquip ? AttachPoint.GripSocketName : AttachPoint.UnEquipSocketName;

		// SkeletaMesh has priority
		if (SkelMeshComp->SkeletalMesh)
		{
			SkelMeshComp->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
			SkelMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SkelMeshComp->SetGenerateOverlapEvents(false);
		}
		// don't have skeletal -> use static mesh
		else if (StaticMeshComp->GetStaticMesh())
		{
			StaticMeshComp->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
			StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			StaticMeshComp->SetGenerateOverlapEvents(false);
		}

		SetVisibility(true);
	}
}

void ACSWeapon::DetachMeshFromCharacter()
{
//	if (IsValid(CachedCharacter))
	{
		if (SkelMeshComp->SkeletalMesh)
		{
			SkelMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		}
		
		if (StaticMeshComp->GetStaticMesh())
		{
			StaticMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		}

		SetVisibility(false);
	}
}

void ACSWeapon::SetVisibility(bool bVisible)
{
	if (SkelMeshComp->SkeletalMesh)
	{
		SkelMeshComp->SetVisibility(bVisible);
	}

	if (StaticMeshComp->GetStaticMesh())
	{
		StaticMeshComp->SetVisibility(bVisible);
	}
}

UPrimitiveComponent* ACSWeapon::GetMeshComponent()
{
	if (SkelMeshComp->SkeletalMesh)
	{
		return SkelMeshComp;
	}

	if (StaticMeshComp->GetStaticMesh())
	{
		return StaticMeshComp;
	}

	return nullptr;
}

void ACSWeapon::SetOwningPawn(ACSCharacter* NewOwner)
{
	if (GetOwner() != NewOwner)
	{
		Instigator = NewOwner;
		SetOwner(NewOwner);
		SetCachedCharacter(NewOwner);
	}
}

void ACSWeapon::SetCachedCharacter(AActor* NewOwner)
{
	ACSCharacter* Character = NewOwner ? Cast<ACSCharacter>(NewOwner) : nullptr;

	CachedCharacter = Character;
	if (CachedCharacter)
	{
		OnEnterWeaponInventory(CachedCharacter);
	}
	else
	{
		OnLeaveWeaponInventory();
	}
}

void ACSWeapon::OnRep_Owner()
{
	SetCachedCharacter(GetOwner());
}

void ACSWeapon::StartFire(const uint8 FireModeNum)
{
	if (CanFire())
	{
		// @TODO : SetPendingFire
		SetWeaponState(EWeaponState::Firing);

		if (Role < ROLE_Authority)
		{
			ServerStartFire(FireModeNum);
		}
	}
}

void ACSWeapon::StopFire(const uint8 FireModeNum)
{
	// @TODO : ClearPendingFire

	if (IsFiring())
	{
		SetWeaponState(EWeaponState::Active);

		if (Role < ROLE_Authority)
		{
			ServerStopFire(FireModeNum);
		}
	}
}

bool ACSWeapon::CanFire()
{
	bool bValidStateToFire = IsValidStateToFire();

	return bValidStateToFire;
}

bool ACSWeapon::IsFiring() const
{
	return CurrentState == EWeaponState::Firing;
}

bool ACSWeapon::IsValidStateToFire() const
{
	switch (CurrentState)
	{
	case EWeaponState::Active:
	case EWeaponState::Firing:
		return true;
	}

	return false;
}

void ACSWeapon::ServerStartFire_Implementation(const uint8 FireModeNum)
{
	StartFire(FireModeNum);
}

bool ACSWeapon::ServerStartFire_Validate(const uint8 FireModeNum)
{
	return true;
}

void ACSWeapon::ServerStopFire_Implementation(const uint8 FireModeNum)
{
	StopFire(FireModeNum);
}

bool ACSWeapon::ServerStopFire_Validate(const uint8 FireModeNum)
{
	return true;
}

void ACSWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	uint8 OldState = CurrentState;

	if (CurrentState != NewState)
	{
		CurrentState = NewState;

		OnWeaponStateChanged(OldState, NewState);
	}
}

void ACSWeapon::OnWeaponStateChanged(uint8 OldState, uint8 NewState)
{
	// End Old State
	if (CurrentStateNode)
	{
		CurrentStateNode->EndState();
	}
	
	CurrentStateNode = WeaponStateMap.FindRef(static_cast<EWeaponState::Type>(NewState));

	// Begin New State
	if (CurrentStateNode)
	{
		CurrentStateNode->BeginState();
	}

	PlayCurrentStateAnimation();
}

void ACSWeapon::PlayCurrentStateAnimation()
{
	switch (CurrentState)
	{
	case EWeaponState::Active:
		break;
	case EWeaponState::Firing:
		break;
	case EWeaponState::Inactive:
		break;
	case EWeaponState::Reloading:
		break;
	case EWeaponState::Equipping:
		PlayWeaponAnimation(EquipAnim);
		break;
	default:
		break;
	}
}

float ACSWeapon::GetStateTransitionTime() const
{
	float TransitionTime = 0.f;

	switch (CurrentState)
	{
	case EWeaponState::Active:
		break;
	case EWeaponState::Firing:
		break;
	case EWeaponState::Inactive:
		break;
	case EWeaponState::Reloading:
		TransitionTime = ReloadTime;
		break;
	case EWeaponState::Equipping:
		TransitionTime = EquipTime;
		break;
	default:
		break;
	}

	return TransitionTime;
}

void ACSWeapon::BringUpFinished()
{
	SetWeaponState(EWeaponState::Active);
}

void ACSWeapon::PutDown()
{
	SetWeaponState(EWeaponState::Inactive);
}

void ACSWeapon::FireWeapon(bool bWantsToFire)
{
	if (FiringAction)
	{
		if (bWantsToFire)
		{
			FiringAction->StartFire();
		}
		else
		{
			FiringAction->StopFire();
		}
	}
}

void ACSWeapon::PlayFiringAnimation(const int32 FireIndex)
{
	if (FireAnims.IsValidIndex(FireIndex))
	{
		PlayWeaponAnimation(FireAnims[FireIndex]);
	}
	else if (FireAnims.Num() > 0)
	{
		PlayWeaponAnimation(FireAnims[FireAnims.Num() - 1]);
	}
}

void ACSWeapon::PlayWeaponAnimation(const FWeaponAnim& InWeaponAnim)
{
	if (IsValid(CachedCharacter))
	{
		CachedCharacter->PlayAnimMontage(InWeaponAnim.Character3P);
	}

	if (SkelMeshComp)
	{
		UAnimInstance* WeaponAnimInstance = SkelMeshComp->GetAnimInstance();
		if (WeaponAnimInstance)
		{
			WeaponAnimInstance->Montage_Play(InWeaponAnim.Weapon3P);
		}
	}
}

FVector ACSWeapon::GetCameraStartLocation(const FVector& AimDir) const
{
	ACSPlayerController* const PC = CachedCharacter ? Cast<ACSPlayerController>(CachedCharacter->Controller) : nullptr;

	if (IsValid(PC))
	{
		FVector OutLocation;
		FRotator UnusedRotation;
		PC->GetPlayerViewPoint(OutLocation, UnusedRotation);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutLocation = OutLocation + AimDir * ((Instigator->GetActorLocation() - OutLocation) | AimDir);

		return OutLocation;
	}

	return FVector::ZeroVector;
}

FVector ACSWeapon::GetAdjustedAim() const
{
	ACSPlayerController* const PC = CachedCharacter ? Cast<ACSPlayerController>(CachedCharacter->Controller) : nullptr;

	FVector OutAimVector = FVector::ZeroVector;

	if (IsValid(PC))
	{
		FVector UnusedLocation;
		FRotator OutRotation;
		PC->GetPlayerViewPoint(UnusedLocation, OutRotation);

		OutAimVector = OutRotation.Vector();
	}
	else if (Instigator)
	{
		OutAimVector = Instigator->GetBaseAimRotation().Vector();
	}

	return OutAimVector;
}

FVector ACSWeapon::GetMuzzleLocation(const FVector& AimDir, const int32 ComboCount /*= 0*/) const
{
	if (IsValid(SkelMeshComp) && SkelMeshComp->SkeletalMesh)
	{
		return SkelMeshComp->GetSocketLocation(MuzzleSocketName);
	}
	else if (IsValid(StaticMeshComp) && StaticMeshComp->GetStaticMesh())
	{
		return StaticMeshComp->GetSocketLocation(MuzzleSocketName);
	}

	return FVector::ZeroVector;
}

bool ACSWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, FHitResult& OutHit)
{
	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(WeaponTrace), true);
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Instigator);
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace, EndTrace, COLLISION_WEAPON, CollisionParams);

	return OutHit.bBlockingHit;
}

bool ACSWeapon::IsLocallyControlled()
{
	return CachedCharacter && CachedCharacter->IsLocallyControlled();
}

void ACSWeapon::OnRep_CurrentState(uint8 OldState)
{
	OnWeaponStateChanged(OldState, CurrentState);
}

void ACSWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACSWeapon, CurrentState, COND_SkipOwner);
}