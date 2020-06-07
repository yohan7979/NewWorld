// copyrighted by swonlee ... twiclok@naver.com


#include "CSDismembermentCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"

ACSDismembermentCharacter::ACSDismembermentCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkeletalMeshMap.Emplace(TEXT("Head"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("UpperBody"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Hips"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("LowerTorso"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Thigh_L"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Thigh_R"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Calf_L"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Calf_R"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Foot_L"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Foot_R"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Shoulder_L"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Shoulder_R"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("ForeArm_L"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("ForeArm_R"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Hand_L"), nullptr);
	SkeletalMeshMap.Emplace(TEXT("Hand_R"), nullptr);
}

void ACSDismembermentCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACSDismembermentCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACSDismembermentCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	USkeletalMeshComponent* ParentMesh = GetMesh();
	if (IsValid(ParentMesh))
	{
		for (const TPair<FName, USkeletalMesh*>& MeshPair : SkeletalMeshMap)
		{
			if (MeshPair.Key != NAME_None && MeshPair.Value != nullptr)
			{
				USkeletalMeshComponent* NewComponent = NewObject<USkeletalMeshComponent>(this, MeshPair.Key);
				if (NewComponent)
				{
					NewComponent->AttachToComponent(ParentMesh, FAttachmentTransformRules::KeepRelativeTransform);
					NewComponent->SetSkeletalMesh(MeshPair.Value);
					NewComponent->SetComponentTickEnabled(false);
					NewComponent->SetMasterPoseComponent(ParentMesh, true);
					NewComponent->RegisterComponent();

					SkeletalMeshComponents.AddUnique(NewComponent);
				}
			}
		}
	}
}
