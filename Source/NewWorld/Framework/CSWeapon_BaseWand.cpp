// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon_BaseWand.h"
#include "CSCharacter.h"
#include "Components/SkeletalMeshComponent.h"

ACSWeapon_BaseWand::ACSWeapon_BaseWand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttachPoint.GripSocketName = TEXT("wand_use");
	AttachPoint.UnEquipSocketName = TEXT("wand");
}

FVector ACSWeapon_BaseWand::GetMuzzleLocation(const int32 ComboCount /*=0*/) const
{
	if (IsValid(CachedCharacter) && CachedCharacter->GetMesh())
	{
		const FName& DesiredSocketName = (ComboCount % 2 == 1) ? TEXT("magic_right_hand") : TEXT("magic_left_hand");
		return CachedCharacter->GetMesh()->GetSocketLocation(DesiredSocketName);
	}

	return Super::GetMuzzleLocation(ComboCount);
}
