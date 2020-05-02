#pragma once
#include "Animation/BlendSpace.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "CSTypes.generated.h"

DECLARE_DELEGATE_OneParam(FBindIntegerDelegate, int32)
DECLARE_DELEGATE_OneParam(FBindBoolDelegate, bool)

#define COLLISION_WEAPON ECC_GameTraceChannel1

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