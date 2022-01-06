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

	virtual bool IsGameInputAllowed() const;

	void SetIgnoreFireInput(bool bNewFireInput);
	void ResetIgnoreFireInput();
	bool IsFireInputIgnored() const;

	DECLARE_EVENT_OneParam(ACSPlayerController, FOnAcknowledgePossession, APawn*)
	FOnAcknowledgePossession& OnAcknowledgePossession() { return AcknowledgePossessionEvent; }

	static FName InventoryComponentName;

	UFUNCTION(Reliable, Client)
	void ClientMatchInProgress();

	UFUNCTION(BlueprintCallable)
	void UnEquipInventoryItem(const int32 From, const int32 To);

	UFUNCTION(BlueprintCallable)
	void EquipInventoryItem(const int32 From, const int32 To);

	UFUNCTION(BlueprintCallable)
	void MoveInventoryItem(const int32 From, const int32 To);

	UFUNCTION(BlueprintCallable)
	void UseInventoryItem(const int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DropInventoryItem(const int32 SlotIndex);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipInventoryItem(const int32 From, const int32 To);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUnEquipInventoryItem(const int32 From, const int32 To);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerMoveInventoryItem(const int32 From, const int32 To);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUseInventoryItem(const int32 SlotIndex);
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDropInventoryItem(const int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	class UCSAttributeComponent* GetCurrentAttributeComponent() const;

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
	
	uint8 IgnoreFireInput;
};