// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWidgetBase.h"
#include "Framework/CSPlayerController.h"
#include "Framework/CSInventoryManager.h"

UCSWidgetBase::UCSWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	OwningPlayer = GetOwningPlayer<ACSPlayerController>();
	SubscribeViewTargetControllerDelegate(true);
}

void UCSWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	SubscribeViewTargetControllerDelegate(false);
	OwningPlayer = nullptr;
}

void UCSWidgetBase::SubscribeViewTargetControllerDelegate(bool bSubscribe)
{
	if (IsValid(OwningPlayer))
	{
		bool bIsAlreayBound = OwningPlayer->OnControllerSetPawn().IsBoundToObject(this);
		if (!bIsAlreayBound && bSubscribe)
		{
			OwningPlayer->OnControllerSetPawn().AddUObject(this, &UCSWidgetBase::OnControllerSetPawn);

			// for standalone
			if (OwningPlayer->GetPawn() != nullptr)
			{
				OnControllerSetPawn(OwningPlayer->GetPawn());
			}
		}
		else if (bIsAlreayBound && !bSubscribe)
		{
			OwningPlayer->OnControllerSetPawn().RemoveAll(this);
		}
	}
}

void UCSWidgetBase::OnControllerSetPawn(APawn* InPawn)
{

}
