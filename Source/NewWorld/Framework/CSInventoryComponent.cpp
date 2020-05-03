// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventoryComponent.h"

UCSInventoryComponent::UCSInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSInventoryComponent::Initialize(int32 InventorySize)
{
	InventoryItems.Empty(InventorySize);
}

void UCSInventoryComponent::SetInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex)
{
	if (InventoryItems.IsValidIndex(SlotIndex))
	{
		InventoryItems[SlotIndex] = NewItem;
	}
}

FInventoryItem UCSInventoryComponent::GetInventoryItem(int32 SlotIndex) const
{
	if (InventoryItems.IsValidIndex(SlotIndex))
	{
		return InventoryItems[SlotIndex];
	}

	return FInventoryItem();
}

const TArray<FInventoryItem>& UCSInventoryComponent::GetInventoryItems(int32 SlotIndex) const
{
	return InventoryItems;
}

void UCSInventoryComponent::LoadInventoryItems(const TArray<FInventoryItem>& ItemLists, int32 InventorySize)
{
	InventoryItems.Reset(InventorySize);

	for (int32 i = 0 ; i < InventorySize ; ++i)
	{
		SetInventoryItem(ItemLists[i], i);
	}
}

void UCSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}