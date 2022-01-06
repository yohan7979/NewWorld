// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipableCharacter.h"
#include "Components/SkeletalMeshComponent.h"

ACSEquipableCharacter::ACSEquipableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Body_Head	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Body_Head"));
	Body_Chest	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Body_Chest"));
	Body_Hands	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Body_Hands"));
	Body_Legs	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Body_Legs"));
	Body_Feet	= ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Body_Feet"));

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

	for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MainHand); ++i)
	{
		USkeletalMeshComponent* TargetMeshComp = GetSkeletalMeshByType(static_cast<EEquipmentSlot>(i));
		if (TargetMeshComp)
		{
			TargetMeshComp->SetupAttachment(GetMesh());
			TargetMeshComp->SetComponentTickEnabled(false);
			TargetMeshComp->SetMasterPoseComponent(GetMesh());
		}
	}
}

void ACSEquipableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACSEquipableCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
			
	for (const auto& MeshPair : DefaultBodyMeshMap)
	{
		if (MeshPair.Value != nullptr)
		{
			USkeletalMeshComponent* BodyMeshComponent = GetSkeletalMeshByType(MeshPair.Key);
			if (BodyMeshComponent)
			{
				//BodyMeshComponent->SetSkeletalMesh(MeshPair.Value);
				//SetAttachedSkeletalMesh(MeshPair.Value, static_cast<int32>(MeshPair.Key));
			}
		}
	}	
}

// Client
void ACSEquipableCharacter::OnRep_AttachedSkelMeshes()
{
	// until EEquipmentSlot::MainHand
	for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MainHand); ++i)
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
	case EEquipmentSlot::Body_Head:		return Body_Head;
	case EEquipmentSlot::Body_Chest:	return Body_Chest;
	case EEquipmentSlot::Body_Hands:	return Body_Hands;
	case EEquipmentSlot::Body_Legs:		return Body_Legs;
	case EEquipmentSlot::Body_Feet:		return Body_Feet;
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
	case EEquipmentSlot::MainHand:		//return MainHand;
	case EEquipmentSlot::OffHand:		//return OffHand;
	default: break;
	}
	
	return nullptr;
}

void ACSEquipableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSEquipableCharacter, AttachedSkelMeshes);
}
