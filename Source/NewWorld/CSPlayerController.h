// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void StartFire(uint8 FireModeNum /* = 0 */) override;
};
