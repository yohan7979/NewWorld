// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewWorld/Framework/CSInventoryComponent.h"
#include "CSEquipInventoryComponent.generated.h"

class ACSEquipableCharacter;
/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSEquipInventoryComponent : public UCSInventoryComponent
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Initialize(int32 InventorySize) override;
	virtual void SetOwnerCharacter(APawn* InPawn) override;
	virtual void SetInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex) override;

protected:
	virtual void UpdateEquippedMesh(const FInventoryItem& NewItem, int32 SlotIndex);
	virtual void UpdateWeaponEquippedMesh(const FInventoryItem& NewItem, int32 SlotIndex);
	virtual void UpdateItemStatus();

	UPROPERTY(Transient)
	ACSEquipableCharacter* OwnerEquipableCharacter;
};
