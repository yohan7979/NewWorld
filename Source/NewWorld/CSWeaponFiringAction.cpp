// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeaponFiringAction.h"

UCSWeaponFiringAction::UCSWeaponFiringAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponFiringAction::Init(ACSWeapon* InWeapon)
{
	OwnerWeaponPrivate = InWeapon;
}
