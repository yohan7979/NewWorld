// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSWeaponFiringAction.h"
#include "NewWorld/Framework/CSProjectile.h"
#include "CSWeaponFiringAction_Projectile.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, editinlinenew, Within = CSWeapon)
class NEWWORLD_API UCSWeaponFiringAction_Projectile : public UCSWeaponFiringAction
{
	GENERATED_UCLASS_BODY()

public:
	virtual bool CanRefire() override;
	virtual void FireShot() override;
	void InternalFireShot();

	virtual void SpawnProjectile(class ACSWeapon* OwnerWeapon, const FVector& Origin, const FVector& ShotDirection);

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ShowOnlyInnerProperties))
	FProjectileWeaponConfig ProjectileWeaponConfig;

	UPROPERTY(EditDefaultsOnly)
	float SpawnDelayTime;
	FTimerHandle TimerHandle_SpawnDelayTimer;
};
