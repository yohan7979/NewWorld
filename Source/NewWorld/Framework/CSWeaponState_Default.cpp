// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeaponState_Default.h"
#include "CSCharacter.h"


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
	Super::BeginState();

	// Check Ammo (do reload)
}

void UCSWeaponStateFiring::EndState()
{
	Super::EndState();

	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		OwnerWeapon->FireWeapon(false);

		ACSCharacter* OwnerPawn = Cast<ACSCharacter>(OwnerWeapon->GetOwner());
		if (IsValid(OwnerPawn))
		{
			OwnerPawn->SetOrientRotationMode(true, 1.5f);
		}
	}
}

void UCSWeaponStateFiring::OnStateTransitionFinished()
{
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		OwnerWeapon->FireWeapon(true);

		ACSCharacter* OwnerPawn = Cast<ACSCharacter>(OwnerWeapon->GetOwner());
		if (IsValid(OwnerPawn))
		{
			OwnerPawn->SetOrientRotationMode(false);
		}		
	}
}

UCSWeaponStateReloading::UCSWeaponStateReloading(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponStateReloading::OnStateTransitionFinished()
{

}
