// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidgetBase.h"
#include "NewWorld/Framework/CSPlayerController.h"
#include "NewWorld/Framework/CSInventoryManager.h"
#include "NewWorld/Framework/Component/CSInventoryComponent.h"

UCSWidgetBase::UCSWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	OwningPlayer = GetOwningPlayer<ACSPlayerController>();
	SubscribePlayerControllerDelegate(true);
}

void UCSWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	SubscribePlayerControllerDelegate(false);
	OwningPlayer = nullptr;
}

void UCSWidgetBase::SubscribePlayerControllerDelegate(bool bSubscribe)
{
	if (IsValid(OwningPlayer))
	{
		bool bIsAlreadyBound = OwningPlayer->OnAcknowledgePossession().IsBoundToObject(this);
		if (!bIsAlreadyBound && bSubscribe)
		{
			OwningPlayer->OnAcknowledgePossession().AddUObject(this, &UCSWidgetBase::OnAcknowledgePossession);

			// for standalone
			if (OwningPlayer->GetPawn() != nullptr)
			{
				OnAcknowledgePossession(OwningPlayer->GetPawn());
			}
		}
		else if (bIsAlreadyBound && !bSubscribe)
		{
			OwningPlayer->OnAcknowledgePossession().RemoveAll(this);
		}

		UCSInventoryComponent* InvComponent = OwningPlayer ? OwningPlayer->InventoryComponent : nullptr;
		if (InvComponent)
		{
			bIsAlreadyBound = InvComponent->OnItemInfomationUpdate().IsBoundToObject(this);
			if (!bIsAlreadyBound && bSubscribe)
			{
				InvComponent->OnItemInfomationUpdate().AddUObject(this, &UCSWidgetBase::OnItemInfomationUpdate);

				// for standalone
				//if (OwningPlayer->GetPawn() != nullptr)
				{
					OnItemInfomationUpdate(InvComponent->ItemInfomations.Items);
				}
			}
			else if (bIsAlreadyBound && !bSubscribe)
			{
				InvComponent->OnItemInfomationUpdate().RemoveAll(this);
			}
		}
	}
}

void UCSWidgetBase::OnItemInfomationUpdate(const TArray<struct FItemInfomation>& ItemInfomations)
{

}

void UCSWidgetBase::OnAcknowledgePossession(APawn* InPawn)
{

}

class ACSPlayerController* UCSWidgetBase::GetOwningCSPlayer()
{
	if (!OwningPlayer)
	{
		OwningPlayer = GetOwningPlayer<ACSPlayerController>();
	}

	return OwningPlayer;
}
