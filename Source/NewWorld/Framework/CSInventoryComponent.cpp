// Fill out your copyright notice in the Description page of Project Settings.


#include "CSInventoryComponent.h"
#include "CSCharacter.h"

UCSInventoryComponent::UCSInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicated(true);
}

void UCSInventoryComponent::Initialize(int32 InventorySize)
{
	InventoryItems.Empty(InventorySize);
	InventoryItems.AddDefaulted(InventorySize);

	ItemInfomations.Empty(InventorySize);
	ItemInfomations.AddDefaulted(InventorySize);
}

void UCSInventoryComponent::SetOwnerCharacter(APawn* InPawn)
{
	OwnerCharacter = Cast<ACSCharacter>(InPawn);
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

void UCSInventoryComponent::AddInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex)
{
	SetInventoryItem(NewItem, SlotIndex);

	// Update ItemInfomation
	UpdateInventoryItem(SlotIndex);
}

void UCSInventoryComponent::AddInventoryItemAtEmptySlot(const FInventoryItem& NewItem, int32 EmptySlot)
{
	EmptySlot = FMath::Max(EmptySlot, static_cast<int32>(EEquipmentSlot::MAX));

	AddInventoryItem(NewItem, EmptySlot);
}

void UCSInventoryComponent::RemoveInventoryItem(int32 SlotIndex)
{
	SetInventoryItem(FInventoryItem(), SlotIndex);

	// Update ItemInfomation
	UpdateInventoryItem(SlotIndex);
}

// Server
void UCSInventoryComponent::LoadInventoryItems(const TArray<FInventoryItem>& ItemLists, int32 InventorySize)
{
	for (int32 i = 0 ; i < ItemLists.Num(); ++i)
	{
		if (ItemLists.IsValidIndex(i))
		{
			SetInventoryItem(ItemLists[i], i);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%d item index is not valid."), i);
		}
		
	}

	UpdateInventoryItems();
}

void UCSInventoryComponent::UpdateInventoryItem(int32 SlotIndex)
{
	FItemInfomation ItemInfo;

	ItemInfo.FillFrom(InventoryItems[SlotIndex]);

	// for replicate to owner client
	ItemInfomations[SlotIndex] = ItemInfo;

	BroadcastItemInfomationUpdated();
}

// Server
void UCSInventoryComponent::UpdateInventoryItems()
{
	FItemInfomation ItemInfo;
	for (int32 i = 0 ; i < InventoryItems.Num() ; ++i)
	{
		ItemInfo.FillFrom(InventoryItems[i]);

		// for replicate to owner client
		ItemInfomations[i] = ItemInfo;
	}

	BroadcastItemInfomationUpdated();
}

bool UCSInventoryComponent::IsEquipmentSlotEmpty(EEquipmentSlot SlotType) const
{
	return InventoryItems[static_cast<int32>(SlotType)].ID == NAME_None;
}

bool UCSInventoryComponent::HasInventoryEmptySpace(int32& OutSlotIndex) const
{
	for (int i = static_cast<int32>(EEquipmentSlot::MAX); i < InventoryItems.Num(); ++i)
	{
		if (InventoryItems[i].ID == NAME_None)
		{
			OutSlotIndex = i;
			return true;
		}
	}
	
	return false;
}

// Server & Owning Client
void UCSInventoryComponent::OnRep_ItemInfomation()
{
	BroadcastItemInfomationUpdated();
}

void UCSInventoryComponent::BroadcastItemInfomationUpdated()
{
	if (ItemInfomationUpdateEvent.IsBound())
	{
		ItemInfomationUpdateEvent.Broadcast(ItemInfomations);
	}
}

void UCSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCSInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCSInventoryComponent, ItemInfomations, COND_OwnerOnly);
}