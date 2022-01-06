// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "NewWorld/Framework/CSTypes.h"
#include "NewWorld/Framework/CSTable.h"
#include "CSInventoryComponent.generated.h"

class ACSCharacter; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWWORLD_API UCSInventoryComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Initialize(int32 InventorySize);
	virtual void SetOwnerCharacter(APawn* InPawn);
	virtual void SetInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex);
	virtual FInventoryItem GetInventoryItem(int32 SlotIndex) const;
	virtual const TArray<FInventoryItem>& GetInventoryItems(int32 SlotIndex) const;

	virtual void AddInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex);
	virtual void AddInventoryItemAtEmptySlot(const FInventoryItem& NewItem, int32 EmptySlot);

	virtual void RemoveInventoryItem(int32 SlotIndex);
	virtual void LoadInventoryItems(const TArray<FInventoryItem>& ItemLists, int32 InventorySize);
	
	virtual void UpdateInventoryItem(int32 SlotIndex);
	virtual void UpdateAllInventoryItems();
	virtual bool IsEquipmentSlotEmpty(EEquipmentSlot SlotType) const;
	virtual bool HasInventoryEmptySpace(int32& OutSlotIndex) const;

	DECLARE_EVENT_OneParam(UCSInventoryComponent, FOnItemInfomationUpdate, const TArray<FItemInfomation>&)
	FOnItemInfomationUpdate& OnItemInfomationUpdate() { return ItemInfomationUpdateEvent; }

	void BroadcastItemInfomationUpdated();
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<FInventoryItem> InventoryItems;

	UPROPERTY(Replicated)
	FItemInfomationContainer ItemInfomations;	// sync for server & client (owner only)

protected:
	FOnItemInfomationUpdate ItemInfomationUpdateEvent;

	UPROPERTY(Transient)
	ACSCharacter* OwnerCharacter;
};
