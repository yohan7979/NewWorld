// copyrighted by swonlee ... twiclok@naver.com


#include "CSPickup_Item.h"
#include "CSCharacter.h"
#include "CSPlayerController.h"
#include "CSInventoryManager.h"
#include "Components/StaticMeshComponent.h"

ACSPickup_Item::ACSPickup_Item(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ACSPickup_Item::Initialize(const struct FInventoryItem& InventoryItem)
{
	ItemIntializeInfo = InventoryItem;

	OnRep_ItemIntializeInfo();
}

void ACSPickup_Item::GiveTo(class ACSCharacter* Character)
{
	if (IsValid(Character))
	{
		ACSPlayerController* PlayerController = Cast<ACSPlayerController>(Character->GetController());
		if (PlayerController && PlayerController->InventoryManager)
		{
			bSuccessToGive = PlayerController->InventoryManager->TryToGiveItem(ItemID, Amount);
		}
	}
}

void ACSPickup_Item::PostGiveTo(class ACSCharacter* Character)
{
	if (bSuccessToGive && Amount == 0)
	{
		Destroy();
	}
}

bool ACSPickup_Item::CanBePickedUp(class ACSCharacter* Character)
{
	if (Super::CanBePickedUp(Character))
	{
		if (Amount > 0)
		{
			return true;
		}
	}

	return false;
}

void ACSPickup_Item::OnRep_ItemIntializeInfo()
{
	ItemID = ItemIntializeInfo.ID;
	Amount = ItemIntializeInfo.Amount;

	if (ItemIntializeInfo.WorldMesh)
	{
		StaticMeshComponent->SetStaticMesh(ItemIntializeInfo.WorldMesh);
	}
}

void ACSPickup_Item::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACSPickup_Item, ItemIntializeInfo, COND_InitialOnly);
}