// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSWidgetBase : public UUserWidget
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void SubscribeViewTargetControllerDelegate(bool bSubscribe);
	virtual void OnControllerSetPawn(APawn* InPawn);

	UPROPERTY(Transient)
	class ACSPlayerController* OwningPlayer;
};
