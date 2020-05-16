#pragma once
#include "Animation/BlendSpace.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DataTable.h"
#include "TimerManager.h"
#include "CSTypes.generated.h"

DECLARE_DELEGATE_OneParam(FBindIntegerDelegate, int32)
DECLARE_DELEGATE_OneParam(FBindBoolDelegate, bool)

#define COLLISION_WEAPON ECC_GameTraceChannel1
#define COLLISION_PROJECTILE ECC_GameTraceChannel2
#define COLLISION_INTERACTIVE ECC_GameTraceChannel3

USTRUCT(BlueprintType)
struct FCharacterAnimGraph
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace* StandingWalkBlendSpace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAimOffsetBlendSpace* StandingAimOffset;

	float BlendInTime;

	FCharacterAnimGraph()
		: StandingWalkBlendSpace(nullptr)
		, StandingAimOffset(nullptr)
		, BlendInTime(2.f)
	{}

	bool operator!=(const FCharacterAnimGraph& Other)
	{
		return StandingWalkBlendSpace != Other.StandingWalkBlendSpace ||
			StandingAimOffset != Other.StandingAimOffset;
	}
};

USTRUCT()
struct FParticleComponentSet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UParticleSystem> MovableParticle;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UParticleSystem> SpawnParticle;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UParticleSystem> HitParticle;

	FParticleComponentSet()
		: MovableParticle(nullptr)
		, SpawnParticle(nullptr)
		, HitParticle(nullptr)
	{}
};

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
	Accessory,
	Back,
	Earing,
	Ring,
	Waist,
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
	EEquipmentType EquipmentType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentSlot EquipmentSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class ACSWeapon> WeaponClass;

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
		, EquipmentType(EEquipmentType::Armor)
		, EquipmentSlot(EEquipmentSlot::Head)
		, WeaponClass(nullptr)
	{}
};

USTRUCT(BlueprintType)
struct FItemInfomation : public FFastArraySerializerItem
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

	FItemInfomation(const FItemInfomation& InItemInfo)
		: ID(InItemInfo.ID)
		, Icon(InItemInfo.Icon)
		, Name(InItemInfo.Name)
		, Quality(InItemInfo.Quality)
		, Type(InItemInfo.Type)
		, Amount(InItemInfo.Amount)
	{}

	FItemInfomation& operator=(const FItemInfomation& Other)
	{
		ID = Other.ID;
		Icon = Other.Icon;
		Name = Other.Name;
		Quality = Other.Quality;
		Type = Other.Type;

		return *this;
	}

	void FillFrom(FInventoryItem& InventoryItem);
	void Reset();

	void PreReplicatedRemove(const struct FItemInfomationContainer& InArraySerializer) {}
	void PostReplicatedAdd(const struct FItemInfomationContainer& InArraySerializer);
	void PostReplicatedChange(const struct FItemInfomationContainer& InArraySerializer);

	FORCEINLINE bool operator==(const FItemInfomation& rhs) const
	{
		return ID == rhs.ID;
	}

	FORCEINLINE bool operator!=(const FItemInfomation& rhs) const
	{
		return !(FItemInfomation::operator==(rhs));
	}

private:
	void OnRep_ItemInfomation(const struct FItemInfomationContainer& InArraySerializer);
};

USTRUCT()
struct FItemInfomationContainer : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FItemInfomation> Items;

	TWeakObjectPtr<class UCSInventoryComponent> OwnerInventory;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FItemInfomation, FItemInfomationContainer>(Items, DeltaParms, *this);
	}

	void AddDefaultedItems(int32 Size);
	void SetItemInfomation(const FItemInfomation& InItemInfo, int32 SlotIndex);
};

template<>
struct TStructOpsTypeTraits< FItemInfomationContainer > : public TStructOpsTypeTraitsBase2< FItemInfomationContainer >
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};


USTRUCT(BlueprintType)
struct FItemKey
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName ID;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	FItemKey()
		: ID(NAME_None)
		, Amount(1)
	{}

	FItemKey(const FName& InID, int32 InAmount)
		: ID(InID)
		, Amount(InAmount)
	{}
};

enum class EInputMode : uint8 
{
	UIOnly,
	GameAndUI,
	GameOnly,
};