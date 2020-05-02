// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSWidget_InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSWidget_InventorySlot : public UUserWidget
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeDestruct();

public:
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;
};
