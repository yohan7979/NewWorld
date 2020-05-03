// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidget_Equipment.h"
#include "Framework/CSPlayerController.h"
#include "Framework/CSInventoryManager.h"

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

void UCSWidget_Equipment::OnControllerSetPawn(APawn* InPawn)
{
	SubscribeInventoryManagerDelegate(InPawn != nullptr);
}

void UCSWidget_Equipment::OnEquipmentVisibilityChanged(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
