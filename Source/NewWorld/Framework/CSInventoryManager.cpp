// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventoryManager.h"
#include "CSPlayerController.h"
#include "CSInventoryComponent.h"
#include "Engine/NetDriver.h"

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