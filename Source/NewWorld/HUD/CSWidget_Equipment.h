// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSWidgetBase.h"
#include "CSWidget_Equipment.generated.h"

class UCSWidget_InventorySlot;
enum class EEquipmentSlot : uint8;
/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSWidget_Equipment : public UCSWidgetBase
{
	GENERATED_UCLASS_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void SubscribeInventoryManagerDelegate(bool bSubscribe);
	
	virtual void OnEquipmentVisibilityChanged(bool bVisible);
	virtual void OnAcknowledgePossession(APawn* InPawn) override;
	virtual void OnItemInfomationUpdate(const TArray<struct FItemInfomation>& ItemInfomations);

	void UpdateEquipItemInfomation(const FItemInfomation& ItemInfo, UCSWidget_InventorySlot* TargetSlot);
	UCSWidget_InventorySlot* GetEquipmentItemSlot(EEquipmentSlot EquipmentSlot);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* HeadSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* ShoulderSlot;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* ChestSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* HandsSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* LegsSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* FeetSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* AccessorySlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* BackSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* RightRingSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* LeftRingSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* WaistSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* TrinketSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* MainHandSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCSWidget_InventorySlot* OffHandSlot;
};
