// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventoryManager.h"
#include "CSPlayerController.h"

UCSInventoryManager::UCSInventoryManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSInventoryManager::Initialize(class ACSPlayerController* InOwningPlayer)
{
	OwningPlayer = InOwningPlayer;
}

void UCSInventoryManager::ToggleInventory()
{
	bIsOpened = !bIsOpened;

	if (InventoryVisibilityChangeEvent.IsBound())
	{
		InventoryVisibilityChangeEvent.Broadcast(bIsOpened);
	}

	OnInventoryStateChanged(bIsOpened);
}

void UCSInventoryManager::OnInventoryStateChanged(bool bOpen)
{

}