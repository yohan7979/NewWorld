// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventoryManager.h"
#include "CSPlayerController.h"
#include "CSInventoryComponent.h"
#include "Engine/NetDriver.h"
#include "Engine/DataTable.h"
#include "CSPickup_Item.h"
#include "CSGameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "CSCharacter.h"
#include "CSWeapon.h"

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
		//StartEquipItemKeys.Add(EEquipmentSlot::Head, FItemKey(TEXT("Armor_Leather_Helmet"), 1));
		//StartEquipItemKeys.Add(EEquipmentSlot::Feet, FItemKey(TEXT("Armor_Leather_Boots"), 1));
		
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

bool UCSInventoryManager::TryToGiveItem(const FName& ItemID, int32& Amount)
{
	if (ItemTable && InventoryComponent)
	{
		static const FString ContextString = TEXT("UCSInventoryManager::TryToGiveItem");
		FInventoryItem* ItemToGive = ItemTable->FindRow<FInventoryItem>(ItemID, ContextString);
		if (ItemToGive && Amount > 0)
		{
			// 1. Equipment �� ���, SlotType�� ����ִ��� Ȯ��
			if (ItemToGive->ItemType == EItemType::Equipment && InventoryComponent->IsEquipmentSlotEmpty(ItemToGive->EquipmentSlot))
			{
				// ��� �ִٸ� ���Կ� �ٷ� ����
				InventoryComponent->AddInventoryItem(*ItemToGive, static_cast<int32>(ItemToGive->EquipmentSlot));
				Amount -= 1;
				return true;
			}

			// 2. �κ��丮�� �� ���� �ִ��� Ȯ��
			int32 EmptySlot = INDEX_NONE;
			if (InventoryComponent->HasInventoryEmptySpace(EmptySlot))
			{
				// �� ���� �ε����� SetInventoryItem
				InventoryComponent->AddInventoryItemAtEmptySlot(*ItemToGive, EmptySlot);
				Amount -= 1;
				return true;
			}
		}
	}

	return false;
}

int32 UCSInventoryManager::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	ACSPlayerController* PC = GetOwningPlayer();
	if (IsValid(PC))
	{
		return PC->GetFunctionCallspace(Function, Stack);
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

void UCSInventoryManager::EquipItem(UCSInventoryComponent* FromInventory, UCSInventoryComponent* ToInventory, const int32 FromIndex, const int32 ToIndex)
{
	// check same element
	if (FromInventory == ToInventory && FromIndex == ToIndex)
	{
		return;
	}

	// �巡�� ����� ������� Ȯ��
	const FInventoryItem& FromItem = FromInventory->GetInventoryItem(FromIndex);
	if (FromItem.ItemType == EItemType::Equipment)
	{
		// ��� Ÿ���� �����ϴ� �� Ȯ��
		const FInventoryItem& ToItem = ToInventory->GetInventoryItem(ToIndex);
		if (ToItem.ID != NAME_None)
		{
			// �����ϸ鼭 ������ ���� ��� ���� 
			if (FromItem.EquipmentSlot == ToItem.EquipmentSlot)
			{
				ToInventory->AddInventoryItem(FromItem, ToIndex);
				FromInventory->AddInventoryItem(ToItem, FromIndex);
			}
		}
		// ��� �ִ� ��� ���� �ε��� ������ Ȯ��
		else if (FromItem.EquipmentSlot == static_cast<EEquipmentSlot>(ToIndex))
		{
			ToInventory->AddInventoryItem(FromItem, ToIndex);
			FromInventory->RemoveInventoryItem(FromIndex);
		}
	}

}

void UCSInventoryManager::UnEquipItem(UCSInventoryComponent* FromInventory, UCSInventoryComponent* ToInventory, const int32 FromIndex, const int32 ToIndex)
{
	// check same element
	if (FromInventory == ToInventory && FromIndex == ToIndex)
	{
		return;
	}

	const FInventoryItem& FromItem = FromInventory->GetInventoryItem(FromIndex);
	const FInventoryItem& ToItem = ToInventory->GetInventoryItem(ToIndex);
	
	// ��� Ÿ���� �����ϴ� �� Ȯ��
	if (ToItem.ID != NAME_None)
	{
		// ��� Ÿ���� ����̰�, ������ ���� ��� ����
		if (ToItem.ItemType == EItemType::Equipment && ToItem.EquipmentSlot == FromItem.EquipmentSlot)
		{
			ToInventory->AddInventoryItem(FromItem, ToIndex);
			FromInventory->AddInventoryItem(ToItem, FromIndex);
		}
	}
	// ��� �ִµ� ���� ������ �ƴ� ��� ����
	else if(ToIndex >= static_cast<int32>(EEquipmentSlot::MAX))
	{
		ToInventory->AddInventoryItem(FromItem, ToIndex);
		FromInventory->RemoveInventoryItem(FromIndex);
	}
}

void UCSInventoryManager::MoveItem(UCSInventoryComponent* FromInventory, UCSInventoryComponent* ToInventory, const int32 FromIndex, const int32 ToIndex)
{
	// check same element
	if (FromInventory == ToInventory && FromIndex == ToIndex)
	{
		return;
	}

	// equipment won't be here
	int32 EquipmentSlotMax = static_cast<int32>(EEquipmentSlot::MAX);
	if (FromIndex < EquipmentSlotMax || ToIndex < EquipmentSlotMax)
	{
		return;
	}

	const FInventoryItem& FromItem = FromInventory->GetInventoryItem(FromIndex);
	const FInventoryItem& ToItem = ToInventory->GetInventoryItem(ToIndex);
	if (ToItem.ID != NAME_None)
	{
		// Swap Item
		ToInventory->AddInventoryItem(FromItem, ToIndex);
		FromInventory->AddInventoryItem(ToItem, FromIndex);
	}
	else
	{
		// Move Item
		ToInventory->AddInventoryItem(FromItem, ToIndex);
		FromInventory->RemoveInventoryItem(FromIndex);
	}
}

void UCSInventoryManager::UseItem(const int32 SlotIndex)
{
	if (!InventoryComponent)
	{
		return;
	}

	const FInventoryItem& InventoryItem = InventoryComponent->GetInventoryItem(SlotIndex);
	if (InventoryItem.ID != NAME_None)
	{
		switch (InventoryItem.ItemType)
		{
		case EItemType::Equipment:
			UseEquipItem(InventoryItem, SlotIndex); // Equip or UnEquip
			break;
		case EItemType::Currency:
			break;
		case EItemType::Consumable:
			break;
		}
	}
}

void UCSInventoryManager::DropItem(const int32 SlotIndex)
{
	if (!InventoryComponent)
	{
		return;
	}

	const FInventoryItem& InventoryItem = InventoryComponent->GetInventoryItem(SlotIndex);
	if (InventoryItem.ID != NAME_None)
	{
		FTransform SpawnTransform;
		GetDropItemTransform(SpawnTransform);

		ACSPickup_Item* DroppedItem = Cast<ACSPickup_Item>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ACSPickup_Item::StaticClass(), SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (DroppedItem)
		{
			DroppedItem->Initialize(InventoryItem);
			UGameplayStatics::FinishSpawningActor(DroppedItem, SpawnTransform);

			InventoryComponent->RemoveInventoryItem(SlotIndex);

			// if weapon, remove from owner's weapon inventory
			if (InventoryItem.EquipmentSlot >= EEquipmentSlot::MainHand)
			{
				FInventoryItem* MutableItem = const_cast<FInventoryItem*>(&InventoryItem);
				SetOwnerDropWeapon(MutableItem->WeaponClass);
			}
		}
	}
}

void UCSInventoryManager::UseEquipItem(const struct FInventoryItem& InventoryItem, const int32 SlotIndex)
{
	int32 DesiredSlotIndex = INDEX_NONE;
	const bool bWantsToEquip = SlotIndex >= static_cast<int32>(EEquipmentSlot::MAX);

	if (bWantsToEquip)
	{
		DesiredSlotIndex = static_cast<int32>(InventoryItem.EquipmentSlot);
		EquipItem(InventoryComponent, InventoryComponent, SlotIndex, DesiredSlotIndex);
	}
	else if (InventoryComponent->HasInventoryEmptySpace(DesiredSlotIndex))
	{
		UnEquipItem(InventoryComponent, InventoryComponent, SlotIndex, DesiredSlotIndex);
	}
}

void UCSInventoryManager::SetOwnerDropWeapon(TSoftClassPtr<ACSWeapon>& SoftWeaponClass)
{
	ACSCharacter* OwnerCharacter = GetOwningPlayer() ? Cast<ACSCharacter>(GetOwningPlayer()->GetPawn()) : nullptr;
	if (OwnerCharacter)
	{
		UClass* WeaponClass = UCSGameplayStatics::GetLoadedClass<ACSWeapon>(SoftWeaponClass);
		ACSWeapon* FindWeapon = WeaponClass ? OwnerCharacter->FindWeaponInventory(WeaponClass) : nullptr;

		if (FindWeapon)
		{
			// ���� �κ��丮���� ����
			OwnerCharacter->RemoveWeapon(FindWeapon);
		}
	}
}

bool UCSInventoryManager::GetDropItemTransform(FTransform& OutTransform) const
{
	APawn* Pawn = GetOwningPlayer() ? GetOwningPlayer()->GetPawn() : nullptr;
	if (Pawn)
	{
		FVector PawnLocation = Pawn->GetActorLocation();
		FRotator PawnRotation = Pawn->GetActorRotation();
		
		// ��¦ �տ� ����
		PawnLocation += Pawn->GetActorForwardVector() * 100.f;

		OutTransform = FTransform(PawnRotation, PawnLocation);
		return true;
	}

	return false;
}

ENetRole UCSInventoryManager::GetOwnerRole() const
{
	if (IsValid(OwningPlayer))
	{
		return OwningPlayer->GetLocalRole();
	}

	return ENetRole::ROLE_None;
}