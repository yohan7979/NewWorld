// copyrighted by swonlee ... twiclok@naver.com


#include "CSPickup_Item.h"
#include "CSCharacter.h"
#include "CSPlayerController.h"
#include "CSInventoryManager.h"

ACSPickup_Item::ACSPickup_Item(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ACSPickup_Item::GiveTo(class ACSCharacter* Character)
{
	if (IsValid(Character))
	{
		ACSPlayerController* PlayerController = Cast<ACSPlayerController>(Character->GetController());
		if (PlayerController && PlayerController->InventoryManager)
		{
			bSucessToGive = PlayerController->InventoryManager->TryToGiveItem(ItemID, Amount);
		}
	}
}

void ACSPickup_Item::PostGiveTo(class ACSCharacter* Character)
{
	if (bSucessToGive)
	{
		Destroy();
	}
}
