// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipInventoryComponent.h"
#include "CSEquipableCharacter.h"
#include "CSGameplayStatics.h"
#include "CSWeapon.h"
#include "CSAttributeComponent.h"

UCSEquipInventoryComponent::UCSEquipInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSEquipInventoryComponent::Initialize(int32 InventorySize)
{
	// default inventory size + equipment size
	const int32 AdjustedInventorySize = InventorySize + static_cast<int32>(EEquipmentSlot::MAX);

	InventoryItems.Empty(AdjustedInventorySize);
	InventoryItems.AddDefaulted(AdjustedInventorySize);

	ItemInfomations.AddDefaultedItems(AdjustedInventorySize);
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
			if (SlotIndex < static_cast<int32>(EEquipmentSlot::MainHand))
			{
				UpdateEquippedMesh(NewItem, SlotIndex);
			}
			else
			{
				UpdateWeaponEquippedMesh(NewItem, SlotIndex);
			}

			UpdateItemStatus();
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

void UCSEquipInventoryComponent::UpdateWeaponEquippedMesh(const FInventoryItem& NewItem, int32 SlotIndex)
{
	if (IsValid(OwnerEquipableCharacter))
	{
		ACSWeapon* NewWeapon = nullptr;
		FInventoryItem* MutableItem = const_cast<FInventoryItem*>(&NewItem);
		
		UClass* NewWeaponClass = !MutableItem->WeaponClass.IsNull() ? UCSGameplayStatics::GetLoadedClass<ACSWeapon>(MutableItem->WeaponClass) : nullptr;
		if (NewWeaponClass)
		{
			// 장착
			NewWeapon = OwnerEquipableCharacter->FindWeaponInventory(NewWeaponClass);
			if (NewWeapon)
			{
				// WeaponInventory에 있는 무기면 그냥 장착
				OwnerEquipableCharacter->EquipWeapon(NewWeapon);
			}
			else
			{
				// 없으면 생성해서 장착
				NewWeapon = OwnerEquipableCharacter->CreateAndGiveWeapon(NewWeaponClass);
				if (NewWeapon)
				{
					OwnerEquipableCharacter->EquipWeapon(NewWeapon);
				}
			}
		}
		else
		{
			// 해제
			OwnerEquipableCharacter->EquipWeapon(nullptr);
		}
	}
}

void UCSEquipInventoryComponent::UpdateItemStatus()
{
	if (IsValid(OwnerEquipableCharacter))
	{
		// calculate all equipments
		FCharacterStatus LocalCharacterStatus;
		for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MAX); ++i)
		{
			LocalCharacterStatus += InventoryItems[i].ItemStatus;
		}

		OwnerEquipableCharacter->SetCharacterStatus(LocalCharacterStatus);
	}
}
