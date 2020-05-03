// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSPlayerController.generated.h"

class UCSInventoryManager;
class UCSInventoryComponent;

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void StartFire(uint8 FireModeNum /* = 0 */) override;

	DECLARE_EVENT_OneParam(ACSPlayerController, FOnControllerSetPawn, APawn*)
	FOnControllerSetPawn& OnControllerSetPawn() { return ControllerSetPawnEvent; }

	static FName InventoryComponentName;

protected:
	virtual void SetupInputComponent() override;
	void ToggleInventory();
	void ToggleEquipment();

public:
	UPROPERTY(Transient)
	UCSInventoryManager* InventoryManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	UCSInventoryComponent* InventoryComponent;

private:
	FOnControllerSetPawn ControllerSetPawnEvent;
};
