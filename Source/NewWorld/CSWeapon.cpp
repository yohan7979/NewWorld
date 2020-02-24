// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "CSCharacter.h"
#include "CSWeaponState.h"

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

	WeaponStateMap.Emplace(EWeaponState::Active, nullptr);
	WeaponStateMap.Emplace(EWeaponState::Inactive, nullptr);
	WeaponStateMap.Emplace(EWeaponState::Firing, nullptr);
	WeaponStateMap.Emplace(EWeaponState::Reloading, nullptr);
	WeaponStateMap.Emplace(EWeaponState::Equipping, nullptr);
}

// Called when the game starts or when spawned
void ACSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentStateNode = WeaponStateMap.FindRef(EWeaponState::Inactive);
}

// Called every frame
void ACSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSWeapon::PostInitializeComponents()
{


	Super::PostInitializeComponents();
}

void ACSWeapon::OnEquip()
{
	AttachMeshToCharacter(true);

	SetWeaponState(EWeaponState::Equipping);
}

void ACSWeapon::OnUnEquip()
{
	AttachMeshToCharacter(false);

	SetWeaponState(EWeaponState::Inactive);
}

void ACSWeapon::OnEnterInventory(ACSCharacter* NewOwner)
{
	AttachMeshToCharacter(false);

	SetOwningPawn(NewOwner);
}

void ACSWeapon::OnLeaveInventory()
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
	if (IsValid(CachedCharacter))
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
		OnEnterInventory(CachedCharacter);
	}
	else
	{
		OnLeaveInventory();
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

	SetWeaponState(EWeaponState::Active);

	if (Role < ROLE_Authority)
	{
		ServerStopFire(FireModeNum);
	}
}

bool ACSWeapon::CanFire()
{
	return true;
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

void ACSWeapon::FireWeapon()
{

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

void ACSWeapon::OnRep_CurrentState(uint8 OldState)
{
	OnWeaponStateChanged(OldState, CurrentState);
}

void ACSWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACSWeapon, CurrentState, COND_SkipOwner);
}