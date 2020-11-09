// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "CSWeaponFiringAction.h"
#include "CSWeaponFiringAction_Melee.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, editinlinenew, Within = CSWeapon)
class NEWWORLD_API UCSWeaponFiringAction_Melee : public UCSWeaponFiringAction
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void StopFire() override;
	virtual void FireShot() override;
	virtual void Tick(float fDeltaTime) override;

private:
	void InternalFireShot();

	UPROPERTY(Transient)
	float ElapsedFireTime;
};
