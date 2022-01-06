// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSAnimNotify_AttachSocket.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NEWWORLD_API UCSAnimNotify_AttachSocket : public UAnimNotify
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	bool bEquip;
};
