// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CSInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCSInteractionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class NEWWORLD_API ICSInteractionInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanInteractWith(class ACSCharacter* Character) = 0;
	virtual void OnInteractWith(class ACSCharacter* Character) = 0;
};
