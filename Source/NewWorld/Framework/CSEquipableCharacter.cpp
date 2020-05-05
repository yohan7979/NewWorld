// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipableCharacter.h"
#include "Components/SkeletalMeshComponent.h"

ACSEquipableCharacter::ACSEquipableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HeadSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("HeadSkelMesh"));
	HeadSkelMesh->SetupAttachment(GetMesh());

	ShoulderSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ShoulderSkelMesh"));
	ShoulderSkelMesh->SetupAttachment(GetMesh());

	ChestSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ChestSkelMesh"));
	ChestSkelMesh->SetupAttachment(GetMesh());

	HandsSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("HandsSkelMesh"));
	HandsSkelMesh->SetupAttachment(GetMesh());

	LegsSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("LegsSkelMesh"));
	LegsSkelMesh->SetupAttachment(GetMesh());

	FeetSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FeetSkelMesh"));
	FeetSkelMesh->SetupAttachment(GetMesh());

	BackSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("BackSkelMesh"));
	BackSkelMesh->SetupAttachment(GetMesh());

	WaistSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WaistSkelMesh"));
	WaistSkelMesh->SetupAttachment(GetMesh());

	AccessorySkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("AccessorySkelMesh"));
	AccessorySkelMesh->SetupAttachment(GetMesh());

	EaringSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("EaringSkelMesh"));
	EaringSkelMesh->SetupAttachment(GetMesh());

	RingSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("RingSkelMesh"));
	RingSkelMesh->SetupAttachment(GetMesh());

	TrinketSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("TrinketSkelMesh"));
	TrinketSkelMesh->SetupAttachment(GetMesh());

	MainHandSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("MainHandSkelMesh"));
	MainHandSkelMesh->SetupAttachment(GetMesh());

	OffHandSkelMesh = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("OffHandSkelMesh"));
	OffHandSkelMesh->SetupAttachment(GetMesh());
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
	case EEquipmentSlot::Head:			return HeadSkelMesh;
	case EEquipmentSlot::Shoulder:		return ShoulderSkelMesh;
	case EEquipmentSlot::Chest:			return ChestSkelMesh;
	case EEquipmentSlot::Hands:			return HandsSkelMesh;
	case EEquipmentSlot::Legs:			return LegsSkelMesh;
	case EEquipmentSlot::Feet:			return FeetSkelMesh;
	case EEquipmentSlot::Back:			return BackSkelMesh;
	case EEquipmentSlot::Waist:			return WaistSkelMesh;
	case EEquipmentSlot::Accessory:		return AccessorySkelMesh;
	case EEquipmentSlot::Earing:		return EaringSkelMesh;
	case EEquipmentSlot::Ring:			return RingSkelMesh;
	case EEquipmentSlot::Trinket:		return TrinketSkelMesh;
	case EEquipmentSlot::MainHand:		return MainHandSkelMesh;
	case EEquipmentSlot::OffHand:		return OffHandSkelMesh;
	default: break;
	}
	
	return nullptr;
}

void ACSEquipableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSEquipableCharacter, AttachedSkelMeshes);
}
