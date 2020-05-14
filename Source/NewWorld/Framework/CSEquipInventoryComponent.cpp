// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipInventoryComponent.h"
#include "CSEquipableCharacter.h"
#include "CSGameplayStatics.h"
#include "CSWeapon.h"

UCSEquipInventoryComponent::UCSEquipInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSEquipInventoryComponent::Initialize(int32 InventorySize)
{
	// default inventory size + equipment size
	const int32 AdjustedInventorySize = InventorySize + static_cast<int32>(EEquipmentSlot::MAX);

	InventoryItems.Empty(AdjustedInventorySize);
	InventoryItems.AddDefaulted(AdjustedInventorySize);

	ItemInfomations.Empty(AdjustedInventorySize);
	ItemInfomations.AddDefaulted(AdjustedInventorySize);
}

void UCSEquipInventoryComponent::SetOwnerCharacter(APawn* InPawn)
{
	Super::SetOwnerCharacter(InPawn);

	OwnerEquipableCharacter = Cast<ACSEquipableCharacter>(InPawn);
}

void UCSEquipInventoryComponent::SetInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex)
{
	Super::SetInventoryItem(NewItem, SlotIndex);

	if (InventoryItems.IsValidIndex(SlotIndex))
	{
		const int32 EquipmentSlotSize = static_cast<int32>(EEquipmentSlot::MAX);
		if (SlotIndex < EquipmentSlotSize)
		{
			UpdateEquippedMesh(NewItem, SlotIndex);
		}
	}
}

void UCSEquipInventoryComponent::UpdateEquippedMesh(const FInventoryItem& NewItem, int32 SlotIndex)
{
	if (IsValid(OwnerEquipableCharacter))
	{
		OwnerEquipableCharacter->SetAttachedSkeletalMesh(NewItem.EquipmentMesh, SlotIndex);
	}
}
