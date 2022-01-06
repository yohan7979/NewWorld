
#include "CSTypes.h"
#include "CSGameplayStatics.h"
#include "NewWorld/Framework/Weapon/CSWeapon.h"
#include "NewWorld/Framework/Component/CSInventoryComponent.h"
#include "CSTable.h"

FItemInfomation::FItemInfomation()
	: ID(NAME_None)
	, Icon(nullptr)
	, Name(NAME_None)
	, Quality(EItemQuality::Common)
	, Type(EItemType::Miscellaneous)
	, Amount(1)
{
}

FItemInfomation::FItemInfomation(const FItemInfomation& InItemInfo)
	: ID(InItemInfo.ID)
	, Icon(InItemInfo.Icon)
	, Name(InItemInfo.Name)
	, Quality(InItemInfo.Quality)
	, Type(InItemInfo.Type)
	, Amount(InItemInfo.Amount)
{
}

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

FString FItemInfomation::GetDebugString()
{
	return FString::Printf(TEXT("ID : %s"), *ID.ToString());
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

FCharacterStatus::FCharacterStatus() 
	: Strength(0)
	, Dexterity(0)
	, Intelligence(0)
	, Armor(0)
	, Health(0)
	, Damage(0)
{

}

FCharacterStatus::FCharacterStatus(const FCharacterStatus& InStatus)
	: Strength(InStatus.Strength)
	, Dexterity(InStatus.Dexterity)
	, Intelligence(InStatus.Intelligence)
	, Armor(InStatus.Armor)
	, Health(InStatus.Health)
	, Damage(InStatus.Damage)
{

}

FTakeHitInfo::FTakeHitInfo()
	: ActualDamage(0.f)
	, DamageTypeClass(nullptr)
	, PawnInstigator(nullptr)
	, DamageCauser(nullptr)
	, DamageEventClassID(0)
	, bKilled(false)
{

}

void FTakeHitInfo::EnsureReplication()
{
	++EnsureReplicationDirty;
}

void FTakeHitInfo::SetDamageEvent(const FDamageEvent& InDamageEvent)
{
	DamageEventClassID = InDamageEvent.GetTypeID();

	switch (DamageEventClassID)
	{
	case FPointDamageEvent::ClassID:
		PointDamageEvent = *(static_cast<FPointDamageEvent const*>(&InDamageEvent));
		break;
	case FRadialDamageEvent::ClassID:
		RadialDamageEvent = *(static_cast<FRadialDamageEvent const*>(&InDamageEvent));
		break;
	default:
		GeneralDamageEvent = InDamageEvent;
		break;
	}

	DamageTypeClass = InDamageEvent.DamageTypeClass;
}

FDamageEvent& FTakeHitInfo::GetDamageEvent()
{
	switch (DamageEventClassID)
	{
	case FPointDamageEvent::ClassID:
		if (false == IsValid(PointDamageEvent.DamageTypeClass))
		{
			PointDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
		}
		return PointDamageEvent;
	case FRadialDamageEvent::ClassID:
		if (false == IsValid(RadialDamageEvent.DamageTypeClass))
		{
			RadialDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
		}
		return RadialDamageEvent;
	default:
		if (false == IsValid(GeneralDamageEvent.DamageTypeClass))
		{
			GeneralDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
		}
		return GeneralDamageEvent;
	}
}
