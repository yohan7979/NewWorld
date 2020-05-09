// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidget_Inventory.h"
#include "Framework/CSPlayerController.h"
#include "Framework/CSInventoryManager.h"
#include "CSWidget_InventorySlot.h"
#include "Framework/CSInventoryComponent.h"
#include "Components/UniformGridSlot.h"
#include "Framework/CSCharacter.h"
#include "Framework/CSGameplayStatics.h"

UCSWidget_Inventory::UCSWidget_Inventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWidget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UCSWidget_Inventory::NativeDestruct()
{
	Super::NativeDestruct();

	InventorySlots.Empty();
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

void UCSWidget_Inventory::OnAcknowledgePossession(APawn* InPawn)
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

	ACSPlayerController* Player = GetOwningCSPlayer();
	if (Player && Player->InventoryManager)
	{
		bool bIsInvOpened = Player->InventoryManager->IsInventoryOpened();
		UCSGameplayStatics::SetInputMode(Player, this, bIsInvOpened ? EInputMode::GameAndUI : EInputMode::GameOnly);
	}
}

void UCSWidget_Inventory::OnInventoryInitialized(int32 InventorySize, int32 InventorySlotsPerRow)
{
	if (IsValid(InventoryGridPanel) && !bIsInitialized) // init only once
	{
		InventoryGridPanel->ClearChildren();

		int32 MaxRow = InventorySlotsPerRow > 0 ? FMath::CeilToInt(InventorySize / (float)InventorySlotsPerRow) : 1;
		CreateInventorySlots(InventorySize, MaxRow, InventorySlotsPerRow);

		bIsInitialized = true;
	}
}

void UCSWidget_Inventory::OnItemInfomationUpdate(const TArray<FItemInfomation>& ItemInfomations)
{
	const int32 EquipmentSlotCount = static_cast<int32>(EEquipmentSlot::MAX);
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots.IsValidIndex(i) && ItemInfomations.IsValidIndex(i + EquipmentSlotCount))
		{
			InventorySlots[i]->ItemInfomation = ItemInfomations[i + EquipmentSlotCount];
		}
	}
}

void UCSWidget_Inventory::CreateInventorySlots(int32 InventorySize, int32 MaxRow, int32 MaxColumn)
{
	const int32 EquipmentSlotCount = static_cast<int32>(EEquipmentSlot::MAX);
	int32 SlotIndex = EquipmentSlotCount;
	for (int32 row = 0; row < MaxRow; ++row)
	{
		for (int32 column = 0; column < MaxColumn; ++column)
		{
			if (SlotIndex > InventorySize + EquipmentSlotCount - 1)
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
