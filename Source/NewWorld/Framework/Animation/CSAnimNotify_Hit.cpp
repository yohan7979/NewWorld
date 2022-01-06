// copyrighted by swonlee ... twiclok@naver.com


#include "CSAnimNotify_Hit.h"
#include "NewWorld/Framework/Character/CSCharacter.h"
#include "NewWorld/Framework/Weapon/CSWeapon.h"
#include "Components/SkeletalMeshComponent.h"

UCSAnimNotify_Hit::UCSAnimNotify_Hit(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSAnimNotify_Hit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
// 	if (MeshComp)
// 	{
// 		ACSCharacter* OwnerCharacter = Cast<ACSCharacter>(MeshComp->GetOwner());
// 		if (IsValid(OwnerCharacter) && OwnerCharacter->Weapon)
// 		{
// 			UWorld* const World = OwnerCharacter->GetWorld();
// 			if (World)
// 			{
// 				OwnerCharacter->Weapon->HitTimes.Add(World->GetTimeSeconds());
// 			}
// 			
// 		}
// 	}
}
