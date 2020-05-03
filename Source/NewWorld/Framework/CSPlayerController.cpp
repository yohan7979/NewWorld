// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerController.h"
#include "CSInventoryManager.h"
#include "CSInventoryComponent.h"
#include "CSEquipInventoryComponent.h"

FName ACSPlayerController::InventoryComponentName(TEXT("InventoryComponent"));

ACSPlayerController::ACSPlayerController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCSEquipInventoryComponent>(ACSPlayerController::InventoryComponentName))
{
	InventoryComponent = ObjectInitializer.CreateDefaultSubobject<UCSInventoryComponent>(this, ACSPlayerController::InventoryComponentName);
}

void ACSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InventoryManager = NewObject<UCSInventoryManager>(this);
	InventoryManager->Initialize(this, InventoryComponent);
}

void ACSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
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
	InputComponent->BindAction(TEXT("ToggleEquipment"), IE_Pressed, this, &ACSPlayerController::ToggleEquipment);
}

void ACSPlayerController::ToggleInventory()
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->ToggleInventory();
	}
}

void ACSPlayerController::ToggleEquipment()
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->ToggleEquipment();
	}
}
