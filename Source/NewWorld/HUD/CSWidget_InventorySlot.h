// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/CSInventoryComponent.h"
#include "CSWidget_InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FItemInfomation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ID;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(BlueprintReadOnly)
	EItemQuality Quality;
	
	UPROPERTY(BlueprintReadOnly)
	EItemType Type;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	FItemInfomation()
		: ID(NAME_None)
		, Icon(nullptr)
		, Name(NAME_None)
		, Quality(EItemQuality::Common)
		, Type(EItemType::Miscellaneous)
		, Amount(1)
	{}
};

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly)
	FItemInfomation ItemInfomation;
};
