// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Framework/CSPickupBase.h"
#include "CSPickup_Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NEWWORLD_API ACSPickup_Item : public ACSPickupBase
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void GiveTo(class ACSCharacter* Character) override;
	virtual void PostGiveTo(class ACSCharacter* Character) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 Amount = 1;

	bool bSucessToGive;
};
