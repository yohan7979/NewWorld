// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "CSAnimNotifyState_Trail.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSAnimNotifyState_Trail : public UAnimNotifyState_Trail
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual UParticleSystem* GetOverridenPSTemplate(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

protected:
	void SpawnParticleSystems(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	/** The particle system to use for this trail. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
	TMap<UParticleSystem*, FName> SubPSTemplates;
};
