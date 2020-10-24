// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "CSAnimNode_ModifyBone.h"
#include <BoneControllers/AnimNode_SkeletalControlBase.h>
#include "EdGraph/EdGraphNodeUtils.h"
#include "CSAnimGraphNode_ModifyBone.generated.h"

/**
 * 
 */
UCLASS()
class UCSAnimGraphNode_ModifyBone : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Setting)
	FCSAnimNode_ModifyBone Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

	virtual const FAnimNode_SkeletalControlBase* GetNode() const override;

private:
	FNodeTitleTextTable CachedNodeTitles;
};
