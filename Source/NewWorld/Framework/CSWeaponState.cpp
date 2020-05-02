// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeaponState.h"
#include "TimerManager.h"

UCSWeaponState::UCSWeaponState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponState::BeginState()
{
	const float StateTransitionTime = GetStateTransitionTime();
	if (StateTransitionTime > 0.f)
	{
		FTimerManager& TimerManager = GetOwnerWeapon()->GetWorldTimerManager();
		TimerManager.SetTimer(TimerHandle_StateTransitionFinished, this, &UCSWeaponState::OnStateTransitionFinished, StateTransitionTime, false);
	}
	else
	{
		OnStateTransitionFinished();
	}
}

void UCSWeaponState::EndState()
{
	FTimerManager& TimerManager = GetOwnerWeapon()->GetWorldTimerManager();
	TimerManager.ClearAllTimersForObject(this);
	TimerManager.ClearTimer(TimerHandle_StateTransitionFinished); // explicit clear
}

float UCSWeaponState::GetStateTransitionTime()
{
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon))
	{
		return OwnerWeapon->GetStateTransitionTime();
	}

	return 0.f;
}
