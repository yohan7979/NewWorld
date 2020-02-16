// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon_BaseWand.h"

ACSWeapon_BaseWand::ACSWeapon_BaseWand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttachPoint.GripSocketName = TEXT("wand_use");
	AttachPoint.UnEquipSocketName = TEXT("wand");
}