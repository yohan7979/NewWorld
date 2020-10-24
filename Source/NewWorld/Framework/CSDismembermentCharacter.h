// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "NewWorld/Framework/CSCharacter.h"
#include "CSDismembermentCharacter.generated.h"

class USkeletalMesh;

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSDismembermentCharacter : public ACSCharacter
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, class USkeletalMesh*> SkeletalMeshMap;

	UPROPERTY(Transient)
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
};
