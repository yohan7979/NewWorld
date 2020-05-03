// Fill out your copyright notice in the Description page of Project Settings.


#include "CSEquipInventoryComponent.h"

UCSEquipInventoryComponent::UCSEquipInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSEquipInventoryComponent::Initialize(int32 InventorySize)
{
	// default inventory size + equipment size
	InventoryItems.Empty(InventorySize + static_cast<int32>(EEquipmentSlot::MAX));
}
