// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeaponState_Default.h"


UCSWeaponStateActive::UCSWeaponStateActive(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponStateActive::BeginState()
{
	Super::BeginState();

	// auto reload
}

UCSWeaponStateInactive::UCSWeaponStateInactive(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

UCSWeaponStateEquipping::UCSWeaponStateEquipping(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponStateEquipping::OnStateTransitionFinished()
{
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		OwnerWeapon->BringUpFinished();
	}
}

UCSWeaponStateFiring::UCSWeaponStateFiring(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponStateFiring::BeginState()
{
	// Check Ammo (do reload)

	Super::BeginState();
}

void UCSWeaponStateFiring::OnStateTransitionFinished()
{
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		OwnerWeapon->FireWeapon();
	}
}
