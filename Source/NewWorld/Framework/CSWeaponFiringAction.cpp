// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeaponFiringAction.h"
#include "TimerManager.h"

UCSWeaponFiringAction::UCSWeaponFiringAction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RefireTime = 0.01f; // defaults
	ComboCount = 0;
}

void UCSWeaponFiringAction::StartFire()
{
	if (CanRefire())
	{
		if (!TimerHandle_RefireCheckTimer.IsValid())
		{
			const float FirstDelay = FMath::Max(LastFiredTime + RefireTime - GetWorld()->GetTimeSeconds(), 0.f);
			GetTimerManager().SetTimer(TimerHandle_RefireCheckTimer, this, &UCSWeaponFiringAction::FireShot, RefireTime, true, FirstDelay);
		}
	}
}

void UCSWeaponFiringAction::StopFire()
{
	if (TimerHandle_RefireCheckTimer.IsValid())
	{
		GetTimerManager().ClearTimer(TimerHandle_RefireCheckTimer);
		//ComboCount = 0;
	}
}

bool UCSWeaponFiringAction::CanRefire()
{
	return false;
}

void UCSWeaponFiringAction::FireShot()
{
	LastFiredTime = GetWorld()->GetTimeSeconds();
	if (ComboCount >= MaxComboCount)
	{
		ComboCount = 0;
	}

	// Play Animation
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		OwnerWeapon->PlayFiringAnimation(ComboCount);
		ComboCount += 1;
	}
}

void UCSWeaponFiringAction::Init(ACSWeapon* InWeapon)
{
	OwnerWeaponPrivate = InWeapon;
}

void UCSWeaponFiringAction::Shutdown()
{
	OwnerWeaponPrivate = nullptr;

	GetTimerManager().ClearAllTimersForObject(this);
}

FTimerManager& UCSWeaponFiringAction::GetTimerManager()
{
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		return OwnerWeapon->GetWorldTimerManager();
	}

	return GetWorld()->GetTimerManager();
}
