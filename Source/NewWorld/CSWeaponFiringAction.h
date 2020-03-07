// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSWeapon.h"
#include "CSWeaponFiringAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, WithIn=CSWeapon)
class NEWWORLD_API UCSWeaponFiringAction : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void FireShot() PURE_VIRTUAL(UCSWeaponFiringAction::FireShot, );
	virtual void Init(ACSWeapon* InWeapon);

	template<typename T>
	ACSWeapon* GetOwnerWeapon()
	{
		return Cast<T>(GetOwnerWeapon());
	}

	ACSWeapon* GetOwnerWeapon()
	{
		if (!OwnerWeaponPrivate)
		{
			OwnerWeaponPrivate = GetOuterACSWeapon();
		}

		return OwnerWeaponPrivate;
	}

private:
	ACSWeapon* OwnerWeaponPrivate;
};
