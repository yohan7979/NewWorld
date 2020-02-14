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

	FCharacterAnimGraph()
		: StandingWalkBlendSpace(nullptr)
		, StandingAimOffset(nullptr)
	{}
};