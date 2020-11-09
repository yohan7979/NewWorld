// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSAnimNotify_Hit.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSAnimNotify_Hit : public UAnimNotify
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
