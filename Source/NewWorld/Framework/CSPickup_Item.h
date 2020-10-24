// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "NewWorld/Framework/CSPickupBase.h"
#include "CSTypes.h"
#include "CSPickup_Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NEWWORLD_API ACSPickup_Item : public ACSPickupBase
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Initialize(const struct FInventoryItem& InventoryItem);
	virtual void GiveTo(class ACSCharacter* Character) override;
	virtual void PostGiveTo(class ACSCharacter* Character) override;

	virtual bool CanBePickedUp(class ACSCharacter* Character) override;

	UFUNCTION()
	void OnRep_ItemIntializeInfo();

protected:
	UPROPERTY(EditAnywhere, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, Category = "Item")
	int32 Amount = 1;

	UPROPERTY(ReplicatedUsing=OnRep_ItemIntializeInfo)
	FInventoryItem ItemIntializeInfo;

	bool bSuccessToGive;
};
