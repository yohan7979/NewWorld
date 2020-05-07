// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CSInteractionInterface.h"
#include "CSPickupBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class NEWWORLD_API ACSPickupBase : public AActor, public ICSInteractionInterface
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void BeginPlay() override;

public:	
	// Interaction Interface Begin
	virtual bool CanInteractWith(class ACSCharacter* Character);
	virtual void OnInteractWith(class ACSCharacter* Character);

	virtual void PreGiveTo(class ACSCharacter* Character);
	virtual void GiveTo(class ACSCharacter* Character);
	virtual void PostGiveTo(class ACSCharacter* Character);

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	bool bCanBePickedup;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	class USoundCue* PickupSound;
};
