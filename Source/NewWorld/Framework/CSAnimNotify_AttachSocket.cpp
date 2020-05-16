// copyrighted by swonlee ... twiclok@naver.com


#include "CSAnimNotify_AttachSocket.h"
#include "CSCharacter.h"
#include "CSWeapon.h"
#include "Components/SkeletalMeshComponent.h"

UCSAnimNotify_AttachSocket::UCSAnimNotify_AttachSocket(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSAnimNotify_AttachSocket::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp)
	{
		ACSCharacter* OwnerPawn = Cast<ACSCharacter>(MeshComp->GetOwner());
		if (OwnerPawn && OwnerPawn->Weapon)
		{
			OwnerPawn->Weapon->AttachMeshToCharacter(bEquip);
		}
	}
}
