#pragma once
#include "Engine/DataTable.h"
#include <Engine/StaticMesh.h>
#include <Engine/SkeletalMesh.h>
#include "CSTypes.h"
#include <UObject/SoftObjectPtr.h>
#include "CSTable.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

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
	bool bIsDroppable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* WorldMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* EquipmentMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WorldMesh_Tid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipmentMesh_Tid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentType EquipmentType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentSlot EquipmentSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class ACSWeapon> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterStatus ItemStatus;

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
		, bIsDroppable(true)
		, WorldMesh(nullptr)
		, EquipmentMesh(nullptr)
		, WorldMesh_Tid(NAME_None)
		, EquipmentMesh_Tid(NAME_None)
		, EquipmentType(EEquipmentType::Armor)
		, EquipmentSlot(EEquipmentSlot::Head)
		, WeaponClass(nullptr)
		, ItemStatus()
	{}

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;
};

USTRUCT(BlueprintType)
struct FMeshTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMeshType MeshType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UObject> ModelMesh;
	
	FMeshTable()
		: MeshType(EMeshType::SkeletalMesh)
		, ModelMesh(nullptr)
	{}
};