// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "CSInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Poor,
	Common,
	UnCommon,
	Rare,
	Epic,
	Legendary,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Miscellaneous,
	Equipment,
	Consumable,
	Currency,
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Armor,
	Weapon,
	Shield,
	Miscellaneuos,
};

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Head,
	Shoulder,
	Chest,
	Hands,
	Legs,
	Feet,
	Back,
	Waist,
	Accessory,
	Earing,
	Ring,
	Trinket,
	MainHand,
	OffHand,
	MAX
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemQuality Quality;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsStackable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bIsStackable"))
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* WorldMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* EquipmentMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentType EquipmentType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentSlot EquipmentSlot;

	FInventoryItem()
		: ID(NAME_None)
		, Icon(nullptr)
		, Name(NAME_None)
		, Description("")
		, Quality(EItemQuality::Common)
		, ItemType(EItemType::Miscellaneous)
		, Amount(1)
		, bIsStackable(false)
		, MaxStackSize(1)
		, WorldMesh(nullptr)
		, EquipmentMesh(nullptr)
		, EquipmentType(EEquipmentType::Armor)
		, EquipmentSlot(EEquipmentSlot::Head)
	{}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWWORLD_API UCSInventoryComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Initialize(int32 InventorySize);
	virtual void SetInventoryItem(const FInventoryItem& NewItem, int32 SlotIndex);
	virtual FInventoryItem GetInventoryItem(int32 SlotIndex) const;
	virtual const TArray<FInventoryItem>& GetInventoryItems(int32 SlotIndex) const;
	virtual void LoadInventoryItems(const TArray<FInventoryItem>& ItemLists, int32 InventorySize);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<FInventoryItem> InventoryItems;
};
