// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSWeaponFiringAction.h"
#include "CSWeaponFiringAction_Instant.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, editinlinenew, Within = CSWeapon)
class NEWWORLD_API UCSWeaponFiringAction_Instant : public UCSWeaponFiringAction
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void FireShot() override;
};
