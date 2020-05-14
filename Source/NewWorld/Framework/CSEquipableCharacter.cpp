// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipableCharacter.h"
#include "Components/SkeletalMeshComponent.h"

ACSEquipableCharacter::ACSEquipableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Head		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Head"));
	Shoulder	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Shoulder"));	
	Chest		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Chest"));
	Hands		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Hands"));
	Legs		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Legs"));
	Feet		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Feet"));
	Back		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Back"));	
	Waist		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Waist"));
	Accessory	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Accessory"));
	Earing		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Earing"));
	Ring		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Ring"));
	Trinket		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Trinket"));
	MainHand	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("MainHand"));
	OffHand		= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("OffHand"));


	for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MAX); ++i)
	{
		USkeletalMeshComponent* TargetMeshComp = GetSkeletalMeshByType(static_cast<EEquipmentSlot>(i));
		if (TargetMeshComp)
		{
			TargetMeshComp->SetupAttachment(GetMesh());
			TargetMeshComp->SetComponentTickEnabled(false);
		}
	}
}

// Client
void ACSEquipableCharacter::OnRep_AttachedSkelMeshes()
{
	for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MAX); ++i)
	{
		USkeletalMeshComponent* TargetMeshComp = GetSkeletalMeshByType(static_cast<EEquipmentSlot>(i));
		if (TargetMeshComp && TargetMeshComp->SkeletalMesh != AttachedSkelMeshes[i])
		{
			TargetMeshComp->SetSkeletalMesh(AttachedSkelMeshes[i]);
			TargetMeshComp->SetMasterPoseComponent(GetMesh(), true);
		}
	}
}

// Server
void ACSEquipableCharacter::SetAttachedSkeletalMesh(USkeletalMesh* MeshToAttach, int32 SlotIndex)
{
	if (AttachedSkelMeshes[SlotIndex] != MeshToAttach)
	{
		AttachedSkelMeshes[SlotIndex] = MeshToAttach;

		if (IsLocallyControlled())
		{
			OnRep_AttachedSkelMeshes();
		}
	}
}

USkeletalMeshComponent* ACSEquipableCharacter::GetSkeletalMeshByType(EEquipmentSlot SlotType) const
{
	switch (SlotType)
	{
	case EEquipmentSlot::Head:			return Head;
	case EEquipmentSlot::Shoulder:		return Shoulder;
	case EEquipmentSlot::Chest:			return Chest;
	case EEquipmentSlot::Hands:			return Hands;
	case EEquipmentSlot::Legs:			return Legs;
	case EEquipmentSlot::Feet:			return Feet;
	case EEquipmentSlot::Back:			return Back;
	case EEquipmentSlot::Waist:			return Waist;
	case EEquipmentSlot::Accessory:		return Accessory;
	case EEquipmentSlot::Earing:		return Earing;
	case EEquipmentSlot::Ring:			return Ring;
	case EEquipmentSlot::Trinket:		return Trinket;
	case EEquipmentSlot::MainHand:		return MainHand;
	case EEquipmentSlot::OffHand:		return OffHand;
	default: break;
	}
	
	return nullptr;
}

void ACSEquipableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSEquipableCharacter, AttachedSkelMeshes);
}
