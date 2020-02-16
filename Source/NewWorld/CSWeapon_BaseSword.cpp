// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon_BaseSword.h"

ACSWeapon_BaseSword::ACSWeapon_BaseSword(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttachPoint.GripSocketName = TEXT("sword_use");
	AttachPoint.UnEquipSocketName = TEXT("sword_back");
}