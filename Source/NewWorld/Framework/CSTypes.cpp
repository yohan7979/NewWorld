
#include "CSTypes.h"
#include "CSGameplayStatics.h"
#include "CSWeapon.h"

void FItemInfomation::FillFrom(FInventoryItem& InventoryItem)
{
	ID = InventoryItem.ID;
	Icon = UCSGameplayStatics::GetLoadedObject<UTexture2D>(InventoryItem.Icon);
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
