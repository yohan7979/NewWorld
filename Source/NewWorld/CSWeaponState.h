// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSWeapon.h"
#include "CSWeaponState.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Within=CSWeapon)
class NEWWORLD_API UCSWeaponState : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	ACSWeapon* GetOwnerWeapon()
	{
		if (!OwnerWeaponPrivate)
		{
			OwnerWeaponPrivate = GetOuterACSWeapon();
		}

		return OwnerWeaponPrivate;
	}

	virtual void BeginState();
	virtual void EndState();
	virtual float GetStateTransitionTime() const;
	virtual void OnStateTransitionFinished() {}

private:
	ACSWeapon* OwnerWeaponPrivate;
	
	FTimerHandle TimerHandle_StateTransitionFinished;
};
