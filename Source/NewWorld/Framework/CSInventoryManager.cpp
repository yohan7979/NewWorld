// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventoryManager.h"
#include "CSPlayerController.h"
#include "CSInventoryComponent.h"
#include "Engine/NetDriver.h"
#include "Engine/DataTable.h"

UCSInventoryManager::UCSInventoryManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UCSInventoryManager::Initialize(class ACSPlayerController* InOwningPlayer, class UCSInventoryComponent* InInventoryComponent)
{
	OwningPlayer = InOwningPlayer;
	InventoryComponent = InInventoryComponent;

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (InventoryComponent)
		{
			InventoryComponent->Initialize(InventorySize);
		}
	}
}

void UCSInventoryManager::ToggleInventory()
{
	bIsInventoryOpened = !bIsInventoryOpened;

	if (InventoryVisibilityChangeEvent.IsBound())
	{
		InventoryVisibilityChangeEvent.Broadcast(bIsInventoryOpened);
	}

	OnInventoryStateChanged(bIsInventoryOpened);
}

void UCSInventoryManager::ToggleEquipment()
{
	bIsEquipmentOpened = !bIsEquipmentOpened;

	if (EquipmentVisibilityChangeEvent.IsBound())
	{
		EquipmentVisibilityChangeEvent.Broadcast(bIsEquipmentOpened);
	}

	OnEquipmentStateChanged(bIsEquipmentOpened);
}

void UCSInventoryManager::OnInventoryStateChanged(bool bOpen)
{

}

void UCSInventoryManager::OnEquipmentStateChanged(bool bOpen)
{

}

void UCSInventoryManager::ServerLoadPlayerItems()
{
	static const FString ContextString = TEXT("UCSInventoryManager::ServerLoadPlayerItems");
	TArray<FInventoryItem> LocalItems;

	if (ItemTable)
	{
		TMap<EEquipmentSlot, FItemKey> StartEquipItemKeys;
		StartEquipItemKeys.Add(EEquipmentSlot::Head, FItemKey(TEXT("Armor_Leather_Helmet"), 1));
		StartEquipItemKeys.Add(EEquipmentSlot::Feet, FItemKey(TEXT("Armor_Leather_Boots"), 1));
		
		for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MAX); ++i)
		{
			FItemKey* ItemKeyPtr = StartEquipItemKeys.Find(static_cast<EEquipmentSlot>(i));
			if (ItemKeyPtr)
			{
				FInventoryItem* FoundItem = ItemTable->FindRow<FInventoryItem>(ItemKeyPtr->ID, ContextString);
				if (FoundItem)
				{
					LocalItems.Add(*FoundItem);
					UE_LOG(LogTemp, Log, TEXT("FoundItem ID : %s"), *ItemKeyPtr->ID.ToString());
				}
				else
				{
					LocalItems.Add(FInventoryItem());
				}
			}
			else
			{
				LocalItems.Add(FInventoryItem());
			}
		}

		TArray<FItemKey> StartItemKeys;
		StartItemKeys.Add(FItemKey(TEXT("Weapon_Bone_Dagger"), 1));
		StartItemKeys.Add(FItemKey(TEXT("Armor_Cardboard_Helmet"), 1));
		StartItemKeys.Add(FItemKey(TEXT("Food_Apple_Poisoned"), 1));

		for (const FItemKey& ItemKey : StartItemKeys)
		{
			FInventoryItem* FoundItem = ItemTable->FindRow<FInventoryItem>(ItemKey.ID, ContextString);
			if (FoundItem)
			{
				LocalItems.Add(*FoundItem);
				UE_LOG(LogTemp, Log, TEXT("FoundItem ID : %s"), *ItemKey.ID.ToString());
			}
		}

		if (IsValid(InventoryComponent))
		{
			InventoryComponent->LoadInventoryItems(LocalItems, InventorySize + static_cast<int32>(EEquipmentSlot::MAX));
		}
	}
}

bool UCSInventoryManager::TryToGiveItem(const FName& ItemID, int32 Amount)
{
	if (ItemTable && InventoryComponent)
	{
		static const FString ContextString = TEXT("UCSInventoryManager::TryToGiveItem");
		FInventoryItem* ItemToGive = ItemTable->FindRow<FInventoryItem>(ItemID, ContextString);
		if (ItemToGive)
		{
			// 1. Equipment 인 경우, SlotType이 비어있는지 확인 (비어 있지 않다면 2번으로)
			if (ItemToGive->ItemType == EItemType::Equipment && InventoryComponent->IsEquipmentSlotEmpty(ItemToGive->EquipmentSlot))
			{
				// 비어 있다면 슬롯에 바로 장착
				InventoryComponent->AddInventoryItem(*ItemToGive, static_cast<int32>(ItemToGive->EquipmentSlot));
				return true;
			}

			// 2. 인벤토리에 빈 공간 있는지 확인 (가득 차 있다면 return false)
			int32 EmptySlot = INDEX_NONE;
			if (InventoryComponent->HasInventoryEmptySpace(EmptySlot))
			{
				// 빈 공간 인덱스에 SetInventoryItem
				InventoryComponent->AddInventoryItemAtEmptySlot(*ItemToGive, EmptySlot);
				return true;
			}
		}
	}

	return false;
}

int32 UCSInventoryManager::GetFunctionCallspace(UFunction* Function, void* Parameters, FFrame* Stack)
{
	ACSPlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		return PC->GetFunctionCallspace(Function, Parameters, Stack);
	}

	return FunctionCallspace::Local;
}

bool UCSInventoryManager::CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack)
{
	ACSPlayerController* PC = GetOwningPlayer();
	UNetDriver* NetDriver = IsValid(PC) ? PC->GetNetDriver() : nullptr;
	if (NetDriver)
	{
		NetDriver->ProcessRemoteFunction(PC, Function, Parms, OutParms, Stack, this);
		return true;
	}

	return false;
}

bool UCSInventoryManager::IsSupportedForNetworking() const
{
	return true;
}

ENetRole UCSInventoryManager::GetOwnerRole() const
{
	if (IsValid(OwningPlayer))
	{
		return OwningPlayer->GetLocalRole();
	}

	return ENetRole::ROLE_None;
}