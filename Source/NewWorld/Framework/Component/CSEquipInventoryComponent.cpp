// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipInventoryComponent.h"
#include "NewWorld/Framework/Character/CSEquipableCharacter.h"
#include "NewWorld/Framework/CSGameplayStatics.h"
#include "NewWorld/Framework/Weapon/CSWeapon.h"
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
		FName MeshTid = NewItem.EquipmentMesh_Tid;
		if (MeshTid.IsNone())
		{
			MeshTid = GetDefaultMeshTid(SlotIndex);
		}

		USkeletalMesh* LoadedMesh = UCSGameplayStatics::GetLoadedMesh<USkeletalMesh>(OwnerEquipableCharacter, MeshTid);
		OwnerEquipableCharacter->SetAttachedSkeletalMesh(LoadedMesh, SlotIndex);
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

FName UCSEquipInventoryComponent::GetDefaultMeshTid(int32 SlotIndex)
{
	switch (static_cast<EEquipmentSlot>(SlotIndex))
	{
	case EEquipmentSlot::Body_Head:
		return TEXT("SK_Body_Face");
	case EEquipmentSlot::Body_Chest:
		return TEXT("SK_Body_Chest");
	case EEquipmentSlot::Body_Hands:
		return TEXT("SK_Body_Hands");
	case EEquipmentSlot::Body_Legs:
		return TEXT("SK_Body_Legs");
// 	case EEquipmentSlot::Body_Feet:
// 		return TEXT("SK_Body_Hands");
	}

	return NAME_None;
}
