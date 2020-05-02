// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidget_Inventory.h"
#include "Framework/CSPlayerController.h"
#include "Framework/CSInventoryManager.h"
#include "CSWidget_InventorySlot.h"
#include "Components/UniformGridSlot.h"
#include "Framework/CSCharacter.h"

UCSWidget_Inventory::UCSWidget_Inventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWidget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);

	OwningPlayer = GetOwningPlayer<ACSPlayerController>();

	SubscribeViewTargetControllerDelegate(true);
}

void UCSWidget_Inventory::NativeDestruct()
{
	Super::NativeDestruct();

	SubscribeViewTargetControllerDelegate(false);

	OwningPlayer = nullptr;
}

void UCSWidget_Inventory::SubscribeViewTargetControllerDelegate(bool bSubscribe)
{
	if (IsValid(OwningPlayer))
	{
		bool bIsAlreayBound = OwningPlayer->OnControllerSetPawn().IsBoundToObject(this);
		if (!bIsAlreayBound && bSubscribe)
		{
			OwningPlayer->OnControllerSetPawn().AddUObject(this, &UCSWidget_Inventory::OnControllerSetPawn);
			 
			// for standalone
			if (OwningPlayer->GetPawn() != nullptr)
			{
				OnControllerSetPawn(OwningPlayer->GetPawn());
			}
		}
		else if (bIsAlreayBound && !bSubscribe)
		{
			OwningPlayer->OnControllerSetPawn().RemoveAll(this);
		}
	}
}

void UCSWidget_Inventory::SubscribeInventoryManagerDelegate(bool bSubscribe)
{
	if (IsValid(OwningPlayer))
	{
		UCSInventoryManager* InvManager = OwningPlayer->InventoryManager;
		if (IsValid(InvManager))
		{
			bool bIsAlreadyBound = InvManager->OnInventoryVisibilityChanged().IsBoundToObject(this);
			if (!bIsAlreadyBound && bSubscribe)
			{
				InvManager->OnInventoryVisibilityChanged().AddUObject(this, &UCSWidget_Inventory::OnInventoryVisibilityChanged);
			}
			else if (bIsAlreadyBound && !bSubscribe)
			{
				InvManager->OnInventoryVisibilityChanged().RemoveAll(this);
			}
		}
	}
}

void UCSWidget_Inventory::OnControllerSetPawn(APawn* InPawn)
{
	SubscribeInventoryManagerDelegate(InPawn != nullptr);

	if (OwningPlayer && InPawn)
	{
		UCSInventoryManager* InvManager = OwningPlayer->InventoryManager;
		if (IsValid(InvManager))
		{
			OnInventoryInitialized(InvManager->InventorySize, InvManager->InventorySlotsPerRow);
		}
	}
}

void UCSWidget_Inventory::OnInventoryVisibilityChanged(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCSWidget_Inventory::OnInventoryInitialized(int32 InventorySize, int32 InventorySlotsPerRow)
{
	if (IsValid(InventoryGridPanel))
	{
		InventoryGridPanel->ClearChildren();

		int32 MaxRow = InventorySlotsPerRow > 0 ? FMath::CeilToInt(InventorySize / (float)InventorySlotsPerRow) : 1;
		CreateInventorySlots(InventorySize, MaxRow, InventorySlotsPerRow);
	}
}

void UCSWidget_Inventory::CreateInventorySlots(int32 InventorySize, int32 MaxRow, int32 MaxColumn)
{
	int32 SlotIndex = 0;
	for (int32 row = 0; row < MaxRow; ++row)
	{
		for (int32 column = 0; column < MaxColumn; ++column)
		{
			if (SlotIndex > InventorySize - 1)
			{
				return;
			}

			AddInventorySlots(row, column, SlotIndex++);
		}
	}
}

void UCSWidget_Inventory::AddInventorySlots(int32 Row, int32 Column, int32 SlotIndex)
{
	if (IsValid(SlotClass))
	{
		UCSWidget_InventorySlot* NewSlotWidget = CreateWidget<UCSWidget_InventorySlot>(this, SlotClass);
		if (IsValid(InventoryGridPanel) && NewSlotWidget)
		{
			UPanelSlot* PanelSlot = InventoryGridPanel->AddChild(NewSlotWidget);
			UUniformGridSlot* UniformGridSlot = PanelSlot ? Cast<UUniformGridSlot>(PanelSlot) : nullptr;
			if (UniformGridSlot)
			{
				UniformGridSlot->SetRow(Row);
				UniformGridSlot->SetColumn(Column);
			}

			NewSlotWidget->SlotIndex = SlotIndex;
			InventorySlots.Add(NewSlotWidget);
		}
	}
	
}