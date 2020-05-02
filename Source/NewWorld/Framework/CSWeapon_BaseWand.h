// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSWeapon.h"
#include "CSWeapon_BaseWand.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NEWWORLD_API ACSWeapon_BaseWand : public ACSWeapon
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual FVector GetMuzzleLocation(const int32 ComboCount = 0) const override;
};
