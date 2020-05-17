// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerController.h"
#include "CSInventoryManager.h"
#include "CSInventoryComponent.h"
#include "CSEquipInventoryComponent.h"
#include "CSCharacter.h"

FName ACSPlayerController::InventoryComponentName(TEXT("InventoryComponent"));

ACSPlayerController::ACSPlayerController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCSEquipInventoryComponent>(ACSPlayerController::InventoryComponentName))
{
	InventoryComponent = ObjectInitializer.CreateDefaultSubobject<UCSInventoryComponent>(this, ACSPlayerController::InventoryComponentName);
}

void ACSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InventoryManagerClass && InventoryManager == nullptr)
	{
		InventoryManager = NewObject<UCSInventoryManager>(this, InventoryManagerClass);
		InventoryManager->Initialize(this, InventoryComponent);
	}
}

void ACSPlayerController::BeginPlay()
{
	Super::BeginPlay();
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

void ACSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn && InventoryComponent)
	{
		InventoryComponent->SetOwnerCharacter(InPawn);
	}
}

void ACSPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	if (AcknowledgePossessionEvent.IsBound())
	{
		AcknowledgePossessionEvent.Broadcast(P);
	}
}

void ACSPlayerController::ServerAcknowledgePossession_Implementation(class APawn* P)
{
	Super::ServerAcknowledgePossession_Implementation(P);

	if (IsValid(InventoryManager))
	{
		InventoryManager->ServerLoadPlayerItems();
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

		bShowMouseCursor = InventoryManager->IsInventoryOpened();
	}
}

void ACSPlayerController::ToggleEquipment()
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->ToggleEquipment();

		bShowMouseCursor = InventoryManager->IsInventoryOpened();
	}
}

void ACSPlayerController::ClientMatchInProgress_Implementation()
{

}

// Local Client
void ACSPlayerController::UnEquipInventoryItem(const int32 From, const int32 To)
{
	if (IsLocalPlayerController())
	{
		ServerUnEquipInventoryItem(From, To);
	}
}

// Local Client
void ACSPlayerController::EquipInventoryItem(const int32 From, const int32 To)
{
	if (IsLocalPlayerController())
	{
		ServerEquipInventoryItem(From, To);
	}
}

// Local Client
void ACSPlayerController::MoveInventoryItem(const int32 From, const int32 To)
{
	if (IsLocalPlayerController())
	{
		ServerMoveInventoryItem(From, To);
	}
}

// Local Client
void ACSPlayerController::UseInventoryItem(const int32 SlotIndex)
{
	if (IsLocalPlayerController())
	{
		ServerUseInventoryItem(SlotIndex);
	}
}

// Local Client
void ACSPlayerController::DropInventoryItem(const int32 SlotIndex)
{
	if (IsLocalPlayerController())
	{
		ServerDropInventoryItem(SlotIndex);
	}
}

void ACSPlayerController::ServerEquipInventoryItem_Implementation(const int32 From, const int32 To)
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->EquipItem(InventoryComponent, InventoryComponent, From, To);
	}
}

bool ACSPlayerController::ServerEquipInventoryItem_Validate(const int32 From, const int32 To)
{
	return true;
}

void ACSPlayerController::ServerUnEquipInventoryItem_Implementation(const int32 From, const int32 To)
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->UnEquipItem(InventoryComponent, InventoryComponent, From, To);
	}
}

bool ACSPlayerController::ServerUnEquipInventoryItem_Validate(const int32 From, const int32 To)
{
	return true;
}

void ACSPlayerController::ServerMoveInventoryItem_Implementation(const int32 From, const int32 To)
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->MoveItem(InventoryComponent, InventoryComponent, From, To);
	}
}

bool ACSPlayerController::ServerMoveInventoryItem_Validate(const int32 From, const int32 To)
{
	return true;
}

void ACSPlayerController::ServerUseInventoryItem_Implementation(const int32 SlotIndex)
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->UseItem(SlotIndex);
	}
}

bool ACSPlayerController::ServerUseInventoryItem_Validate(const int32 SlotIndex)
{
	return true;
}

void ACSPlayerController::ServerDropInventoryItem_Implementation(const int32 SlotIndex)
{
	if (IsValid(InventoryManager))
	{
		InventoryManager->DropItem(SlotIndex);
	}
}

bool ACSPlayerController::ServerDropInventoryItem_Validate(const int32 SlotIndex)
{
	return true;
}

class UCSAttributeComponent* ACSPlayerController::GetCurrentAttributeComponent() const
{
	ACSCharacter* OwnedCharacter = GetPawn() ? Cast<ACSCharacter>(GetPawn()) : nullptr;
	if (IsValid(OwnedCharacter))
	{
		return OwnedCharacter->GetAttributeComponent();
	}

	return nullptr;
}
