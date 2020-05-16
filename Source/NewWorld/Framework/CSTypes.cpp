
#include "CSTypes.h"
#include "CSGameplayStatics.h"
#include "CSWeapon.h"
#include "CSInventoryComponent.h"

void FItemInfomation::FillFrom(FInventoryItem& InventoryItem)
{
	ID = InventoryItem.ID;
	Icon = UCSGameplayStatics::GetLoadedObject<UTexture2D>(InventoryItem.Icon);
	Amount = InventoryItem.Amount;
	Name = InventoryItem.Name;
	Quality = InventoryItem.Quality;
	Type = InventoryItem.ItemType;
}

void FItemInfomation::Reset()
{
	ID = NAME_None;
	Icon = nullptr;
	Amount = 1;
	Name = NAME_None;
	Quality = EItemQuality::Common;
	Type = EItemType::Miscellaneous;
}

void FItemInfomation::PostReplicatedAdd(const struct FItemInfomationContainer& InArraySerializer)
{
	OnRep_ItemInfomation(InArraySerializer);
}

void FItemInfomation::PostReplicatedChange(const struct FItemInfomationContainer& InArraySerializer)
{
	OnRep_ItemInfomation(InArraySerializer);
}

void FItemInfomation::OnRep_ItemInfomation(const struct FItemInfomationContainer& InArraySerializer)
{
	if (InArraySerializer.OwnerInventory.IsValid())
	{
		InArraySerializer.OwnerInventory->BroadcastItemInfomationUpdated();
	}
}

void FItemInfomationContainer::AddDefaultedItems(int32 Size)
{
	for (int32 i = 0; i < Size; ++i)
	{
		MarkItemDirty(Items.AddDefaulted_GetRef());
	}
}

void FItemInfomationContainer::SetItemInfomation(const FItemInfomation& InItemInfo, int32 SlotIndex)
{
	if (Items.IsValidIndex(SlotIndex))
	{
		Items[SlotIndex] = InItemInfo;
		MarkItemDirty(Items[SlotIndex]);
	}
}
