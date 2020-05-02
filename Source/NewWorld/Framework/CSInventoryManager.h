// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSInventoryManager.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSInventoryManager : public UObject
{
	GENERATED_BODY()
	
public:
	UCSInventoryManager(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize(class ACSPlayerController* InOwningPlayer);
	virtual void ToggleInventory();
	virtual void OnInventoryStateChanged(bool bOpen);

	DECLARE_EVENT_OneParam(UCSInventoryManager, FOnInventoryVisibilityChanged, bool)
	FOnInventoryVisibilityChanged& OnInventoryVisibilityChanged() { return InventoryVisibilityChangeEvent; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySize = 28;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventorySlotsPerRow = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Gold;

	UPROPERTY(Transient)
	ACSPlayerController* OwningPlayer;

private:
	FOnInventoryVisibilityChanged InventoryVisibilityChangeEvent;
	bool bIsOpened;
};
