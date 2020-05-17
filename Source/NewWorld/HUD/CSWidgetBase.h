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

	virtual void SubscribePlayerControllerDelegate(bool bSubscribe);

	virtual void OnItemInfomationUpdate(const TArray<struct FItemInfomation>& ItemInfomations);
	virtual void OnAcknowledgePossession(APawn* InPawn);

public:
	class ACSPlayerController* GetOwningCSPlayer();

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	class ACSPlayerController* OwningPlayer;
};
