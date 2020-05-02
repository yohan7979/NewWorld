// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "CSWidget_Inventory.generated.h"

class UCSWidget_InventorySlot;

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSWidget_Inventory : public UUserWidget
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	virtual void SubscribeViewTargetControllerDelegate(bool bSubscribe);
	virtual void SubscribeInventoryManagerDelegate(bool bSubscribe);

	virtual void OnControllerSetPawn(APawn* InPawn);
	virtual void OnInventoryVisibilityChanged(bool bVisible);
	virtual void OnInventoryInitialized(int32 InventorySize, int32 InventorySlotsPerRow);

	virtual void CreateInventorySlots(int32 InventorySize, int32 MaxRow, int32 MaxColumn);
	virtual void AddInventorySlots(int32 Row, int32 Column, int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCSWidget_InventorySlot> SlotClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<UCSWidget_InventorySlot*> InventorySlots;
	
	UPROPERTY(Transient)
	class ACSPlayerController* OwningPlayer;
};
