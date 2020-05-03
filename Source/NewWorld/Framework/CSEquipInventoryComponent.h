// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/CSInventoryComponent.h"
#include "CSEquipInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSEquipInventoryComponent : public UCSInventoryComponent
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Initialize(int32 InventorySize) override;

protected:
};
