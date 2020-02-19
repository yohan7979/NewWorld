#pragma once
#include "Animation/BlendSpace.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "CSTypes.generated.h"

DECLARE_DELEGATE_OneParam(FBindIntegerDelegate, int32)

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