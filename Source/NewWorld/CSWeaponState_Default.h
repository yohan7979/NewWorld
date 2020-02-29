// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSWeaponState.h"
#include "CSWeaponState_Default.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSWeaponStateActive : public UCSWeaponState
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginState() override;
};

UCLASS()
class NEWWORLD_API UCSWeaponStateInactive : public UCSWeaponState
{
	GENERATED_UCLASS_BODY()

public:
};

UCLASS()
class NEWWORLD_API UCSWeaponStateEquipping : public UCSWeaponState
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void OnStateTransitionFinished() override;
};

UCLASS()
class NEWWORLD_API UCSWeaponStateReloading : public UCSWeaponState
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnStateTransitionFinished() override;
};

UCLASS()
class NEWWORLD_API UCSWeaponStateFiring : public UCSWeaponState
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginState() override;
	virtual void OnStateTransitionFinished() override;
};
