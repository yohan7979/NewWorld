// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSWeapon.h"
#include "CSWeaponTypes.h"
#include "CSWeapon_BaseSword.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NEWWORLD_API ACSWeapon_BaseSword : public ACSWeapon
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void PlayWeaponAnimation(const FWeaponAnim& InWeaponAnim) override;

public:
	UPROPERTY(EditDefaultsOnly)
	FMeleeWeaponConfig MeleeWeaponConfig;
};
