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
	void StartFire();
	void StopFire();
	virtual bool CanRefire();
	virtual void FireShot();
	virtual void Init(ACSWeapon* InWeapon);
	virtual void Shutdown();

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

	FTimerManager& GetTimerManager();

protected:
	UPROPERTY(EditAnywhere, meta = (ClampMin=0.01f))
	float RefireTime;
	float LastFiredTime;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 MaxComboCount;
	int32 ComboCount;

private:
	ACSWeapon* OwnerWeaponPrivate;
	FTimerHandle TimerHandle_RefireCheckTimer;
};
