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

// Server
void UCSInventoryComponent::UpdateInventoryItems()
{
	FItemInfomation ItemInfo;
	for (int32 i = 0 ; i < InventoryItems.Num() ; ++i)
	{
		ItemInfo.ID = InventoryItems[i].ID;
		ItemInfo.Icon = InventoryItems[i].Icon;
		ItemInfo.Amount = InventoryItems[i].Amount;
		ItemInfo.Name = InventoryItems[i].Name;
		ItemInfo.Quality = InventoryItems[i].Quality;
		ItemInfo.Type = InventoryItems[i].ItemType;
		ItemInfo.NetDirty++;

		// for replicate to owner client
		ItemInfomations[i] = ItemInfo;
	}

	OnRep_ItemInfomation();
}

// Server & Owning Client
void UCSInventoryComponent::OnRep_ItemInfomation()
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