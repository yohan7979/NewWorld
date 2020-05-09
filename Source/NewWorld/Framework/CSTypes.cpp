
#include "CSTypes.h"

void FItemInfomation::FillFrom(const FInventoryItem& InventoryItem)
{
	ID = InventoryItem.ID;
	Icon = InventoryItem.Icon;
	Amount = InventoryItem.Amount;
	Name = InventoryItem.Name;
	Quality = InventoryItem.Quality;
	Type = InventoryItem.ItemType;
	NetDirty++;
}

void FItemInfomation::Reset()
{
	ID = NAME_None;
	Icon = nullptr;
	Amount = 1;
	Name = NAME_None;
	Quality = EItemQuality::Common;
	Type = EItemType::Miscellaneous;
	NetDirty++;
}
