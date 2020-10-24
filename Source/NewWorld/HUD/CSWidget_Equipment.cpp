// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidget_Equipment.h"
#include "NewWorld/Framework/CSPlayerController.h"
#include "NewWorld/Framework/CSInventoryManager.h"
#include "NewWorld/Framework/CSGameplayStatics.h"
#include "CSWidget_InventorySlot.h"

UCSWidget_Equipment::UCSWidget_Equipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWidget_Equipment::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UCSWidget_Equipment::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCSWidget_Equipment::SubscribeInventoryManagerDelegate(bool bSubscribe)
{
	if (IsValid(OwningPlayer))
	{
		UCSInventoryManager* InvManager = OwningPlayer->InventoryManager;
		if (IsValid(InvManager))
		{
			bool bIsAlreadyBound = InvManager->OnEquipmentVisibilityChanged().IsBoundToObject(this);
			if (!bIsAlreadyBound && bSubscribe)
			{
				InvManager->OnEquipmentVisibilityChanged().AddUObject(this, &UCSWidget_Equipment::OnEquipmentVisibilityChanged);
			}
			else if (bIsAlreadyBound && !bSubscribe)
			{
				InvManager->OnEquipmentVisibilityChanged().RemoveAll(this);
			}
		}
	}
}

void UCSWidget_Equipment::OnEquipmentVisibilityChanged(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	ACSPlayerController* Player = GetOwningCSPlayer();
	if (Player && Player->InventoryManager)
	{
		bool bIsInvOpened = Player->InventoryManager->IsInventoryOpened();
		UCSGameplayStatics::SetInputMode(Player, this, bIsInvOpened ? EInputMode::GameAndUI : EInputMode::GameOnly);
	}
}

void UCSWidget_Equipment::OnAcknowledgePossession(APawn* InPawn)
{
	SubscribeInventoryManagerDelegate(InPawn != nullptr);
}

void UCSWidget_Equipment::OnItemInfomationUpdate(const TArray<struct FItemInfomation>& ItemInfomations)
{
	for (int32 i = 0; i < static_cast<int32>(EEquipmentSlot::MAX); ++i)
	{
		if (ItemInfomations.IsValidIndex(i))
		{
			UpdateEquipItemInfomation(ItemInfomations[i], GetEquipmentItemSlot(static_cast<EEquipmentSlot>(i)));
		}
	}
}

void UCSWidget_Equipment::UpdateEquipItemInfomation(const FItemInfomation& ItemInfo, UCSWidget_InventorySlot* TargetSlot)
{
	if (TargetSlot)
	{
		TargetSlot->ItemInfomation = ItemInfo;
	}
}

UCSWidget_InventorySlot* UCSWidget_Equipment::GetEquipmentItemSlot(EEquipmentSlot EquipmentSlot)
{
	switch (EquipmentSlot)
	{
	case EEquipmentSlot::Head:			return HeadSlot;
	case EEquipmentSlot::Shoulder:		return ShoulderSlot;
	case EEquipmentSlot::Chest:			return ChestSlot;
	case EEquipmentSlot::Hands:			return HandsSlot;
	case EEquipmentSlot::Legs:			return LegsSlot;
	case EEquipmentSlot::Feet:			return FeetSlot;
	case EEquipmentSlot::Back:			return BackSlot;
	case EEquipmentSlot::Waist:			return WaistSlot;
	case EEquipmentSlot::Accessory:		return AccessorySlot;
	case EEquipmentSlot::Earing:		return RightRingSlot;
	case EEquipmentSlot::Ring:			return LeftRingSlot;
	case EEquipmentSlot::Trinket:		return TrinketSlot;
	case EEquipmentSlot::MainHand:		return MainHandSlot;
	case EEquipmentSlot::OffHand:		return OffHandSlot;
	default:							break;
	}

	return nullptr;
}
