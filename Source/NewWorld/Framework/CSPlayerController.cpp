// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerController.h"
#include "CSInventoryManager.h"

ACSPlayerController::ACSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ACSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn != nullptr)
	{
		InventoryManager = NewObject<UCSInventoryManager>(this);
		InventoryManager->Initialize(this);
	}
	
	if (ControllerSetPawnEvent.IsBound())
	{
		ControllerSetPawnEvent.Broadcast(InPawn);
	}
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

void ACSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("ToggleInventory"), IE_Pressed, this, &ACSPlayerController::ToggleInventory);
}

void ACSPlayerController::ToggleInventory()
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->ToggleInventory();
	}
}
