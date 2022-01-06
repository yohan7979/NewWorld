// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSInventoryManager.generated.h"

class UDataTable;
class UCSInventoryComponent;

/**
 * 
 */
UCLASS(Blueprintable, Within = CSPlayerController)
class NEWWORLD_API UCSInventoryManager : public UObject
{
	GENERATED_BODY()
	
public:
	UCSInventoryManager(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize(class ACSPlayerController* InOwningPlayer, class UCSInventoryComponent* InInventoryComponent);
	virtual void ToggleInventory();
	virtual void ToggleEquipment();
	virtual bool IsInventoryOpened() const { return bIsInventoryOpened || bIsEquipmentOpened; }
	virtual void OnInventoryStateChanged(bool bOpen);
	virtual void OnEquipmentStateChanged(bool bOpen);

	virtual void ServerLoadPlayerItems();
	virtual bool TryToGiveItem(const FName& ItemID, int32& Amount);

	// UObject Interface Begin //
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;
	virtual bool IsSupportedForNetworking() const override;
	// UObject Interface End //

	virtual void EquipItem(UCSInventoryComponent* FromInventory, UCSInventoryComponent* ToInventory, const int32 FromIndex, const int32 ToIndex);
	virtual void UnEquipItem(UCSInventoryComponent* FromInventory, UCSInventoryComponent* ToInventory, const int32 FromIndex, const int32 ToIndex);
	virtual void MoveItem(UCSInventoryComponent* FromInventory, UCSInventoryComponent* ToInventory, const int32 FromIndex, const int32 ToIndex);
	virtual void UseItem(const int32 SlotIndex);
	virtual void DropItem(const int32 SlotIndex);

	virtual void UseEquipItem(const struct FInventoryItem& InventoryItem, const int32 SlotIndex);
	virtual void SetOwnerDropWeapon(TSoftClassPtr<class ACSWeapon>& SoftWeaponClass);
	bool GetDropItemTransform(FTransform& OutTransform) const;

	ENetRole GetOwnerRole() const;
	ACSPlayerController* GetOwningPlayer() const
	{
		if (OwningPlayer == nullptr)
		{
			return GetOuterACSPlayerController();
		}

		return OwningPlayer;
	}

	DECLARE_EVENT_OneParam(UCSInventoryManager, FOnInventoryVisibilityChanged, bool)
	FOnInventoryVisibilityChanged& OnInventoryVisibilityChanged() { return InventoryVisibilityChangeEvent; }

	DECLARE_EVENT_OneParam(UCSInventoryManager, FOnEquipmentVisibilityChanged, bool)
	FOnEquipmentVisibilityChanged& OnEquipmentVisibilityChanged() { return EquipmentVisibilityChangeEvent; }

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySize = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySlotsPerRow = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Gold;

	UPROPERTY(Transient)
	ACSPlayerController* OwningPlayer;

	UPROPERTY(Transient)
	UCSInventoryComponent* InventoryComponent;

private:
	FOnInventoryVisibilityChanged InventoryVisibilityChangeEvent;
	FOnEquipmentVisibilityChanged EquipmentVisibilityChangeEvent;
	bool bIsInventoryOpened;
	bool bIsEquipmentOpened;
};
