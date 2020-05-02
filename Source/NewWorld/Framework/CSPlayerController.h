// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSPlayerController.generated.h"

class UCSInventoryManager;

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

protected:
	virtual void SetupInputComponent() override;
	void ToggleInventory();

public:
	UPROPERTY(Transient)
	UCSInventoryManager* InventoryManager;

private:
	FOnControllerSetPawn ControllerSetPawnEvent;
};
