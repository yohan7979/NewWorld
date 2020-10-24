// copyrighted by swonlee ... twiclok@naver.com


#include "CSAnimGraphNode_ModifyBone.h"

#define LOCTEXT_NAMESPACE "A3Nodes"

FText UCSAnimGraphNode_ModifyBone::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("CSTransform Modify Bone"));
}

FText UCSAnimGraphNode_ModifyBone::GetTooltipText() const
{
	return GetNodeTitle(ENodeTitleType::ListView); 
}

const FAnimNode_SkeletalControlBase* UCSAnimGraphNode_ModifyBone::GetNode() const
{
	return &Node;
}

#undef LOCTEXT_NAMESPACE