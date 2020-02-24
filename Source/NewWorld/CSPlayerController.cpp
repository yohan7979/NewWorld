// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerController.h"


ACSPlayerController::ACSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ACSPlayerController::StartFire(uint8 FireModeNum /* = 0 */)
{
	if (IsInState(NAME_Spectating))
	{
		// if(CanBeSpectator)
		ServerViewNextPlayer();
	}
	else if (GetPawn() && !bCinematicMode && !GetWorld()->bPlayersOnly)
	{
		GetPawn()->PawnStartFire(FireModeNum);
	}
}
