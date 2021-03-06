// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidget_InventorySlot.h"
#include "NewWorld/Framework/CSPlayerController.h"

UCSWidget_InventorySlot::UCSWidget_InventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWidget_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	OwningPlayer = GetOwningPlayer<ACSPlayerController>();
}

void UCSWidget_InventorySlot::NativeDestruct()
{
	Super::NativeDestruct();
}
