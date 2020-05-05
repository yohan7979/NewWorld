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
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void StartFire(uint8 FireModeNum /* = 0 */) override;
	
	virtual void SetPawn(APawn* InPawn) override;
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void ServerAcknowledgePossession_Implementation(class APawn* P) override;

	DECLARE_EVENT_OneParam(ACSPlayerController, FOnAcknowledgePossession, APawn*)
	FOnAcknowledgePossession& OnAcknowledgePossession() { return AcknowledgePossessionEvent; }

	static FName InventoryComponentName;

	UFUNCTION(Reliable, Client)
	void ClientMatchInProgress();

protected:
	virtual void SetupInputComponent() override;
	void ToggleInventory();
	void ToggleEquipment();

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCSInventoryManager> InventoryManagerClass;

	UPROPERTY(Transient)
	UCSInventoryManager* InventoryManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	UCSInventoryComponent* InventoryComponent;

private:
	FOnAcknowledgePossession AcknowledgePossessionEvent;
};
