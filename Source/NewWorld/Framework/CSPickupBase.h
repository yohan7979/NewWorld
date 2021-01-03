// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewWorld/Interface/CSInteractionInterface.h"
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
	// Interaction Interface End

	virtual bool CanBePickedUp(class ACSCharacter* Character);
	virtual void PickedUpBy(class ACSCharacter* Character) final;

private:
	virtual void PreGiveTo(class ACSCharacter* Character);
	virtual void GiveTo(class ACSCharacter* Character) PURE_VIRTUAL(ACSPickupBase, );
	virtual void PostGiveTo(class ACSCharacter* Character);

protected:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Item")
	bool bTryPickedUpWhenOverlapped;

	UPROPERTY(EditAnywhere, Category = "Item")
	class USoundCue* PickupSound;
};
