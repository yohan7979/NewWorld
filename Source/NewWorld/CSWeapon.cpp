// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CSCharacter.h"

ACSWeapon::ACSWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	SkelMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	SkelMeshComp->SetupAttachment(RootComponent);
	SkelMeshComp->SetGenerateOverlapEvents(false);
	SkelMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ACSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSWeapon::PostInitializeComponents()
{


	Super::PostInitializeComponents();
}

void ACSWeapon::OnEquip()
{
	AttachMeshToCharacter(true);
}

void ACSWeapon::OnUnEquip()
{
	AttachMeshToCharacter(false);
}

void ACSWeapon::OnEnterInventory(ACSCharacter* NewOwner)
{
	AttachMeshToCharacter(false);

	SetOwningPawn(NewOwner);
}

void ACSWeapon::OnLeaveInventory()
{
	DetachMeshFromCharacter();

	SetOwningPawn(nullptr);
}

void ACSWeapon::AttachMeshToCharacter(bool bEquip)
{
	if (IsValid(CachedCharacter))
	{
		// Detach previous for safe
		DetachMeshFromCharacter();

		USkeletalMeshComponent* PawnMesh = CachedCharacter->GetMesh();
		const FName& AttachSocketName = bEquip ? AttachPoint.GripSocketName : AttachPoint.UnEquipSocketName;

		// SkeletaMesh has priority
		if (SkelMeshComp->SkeletalMesh)
		{
			SkelMeshComp->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
			SkelMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SkelMeshComp->SetGenerateOverlapEvents(false);
		}
		// don't have skeletal -> use static mesh
		else if (StaticMeshComp->GetStaticMesh())
		{
			StaticMeshComp->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
			StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			StaticMeshComp->SetGenerateOverlapEvents(false);
		}

		SetVisibility(true);
	}
}

void ACSWeapon::DetachMeshFromCharacter()
{
	if (IsValid(CachedCharacter))
	{
		if (SkelMeshComp->SkeletalMesh)
		{
			SkelMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		}
		
		if (StaticMeshComp->GetStaticMesh())
		{
			StaticMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		}

		SetVisibility(false);
	}
}

void ACSWeapon::SetVisibility(bool bVisible)
{
	if (SkelMeshComp->SkeletalMesh)
	{
		SkelMeshComp->SetVisibility(bVisible);
	}

	if (StaticMeshComp->GetStaticMesh())
	{
		StaticMeshComp->SetVisibility(bVisible);
	}
}

void ACSWeapon::SetOwningPawn(ACSCharacter* NewOwner)
{
	if (GetOwner() != NewOwner)
	{
		Instigator = NewOwner;
		SetOwner(NewOwner);
		SetCachedCharacter(NewOwner);
	}
}

void ACSWeapon::SetCachedCharacter(AActor* NewOwner)
{
	ACSCharacter* Character = NewOwner ? Cast<ACSCharacter>(NewOwner) : nullptr;
	CachedCharacter = Character;
	if (CachedCharacter)
	{
		OnEnterInventory(CachedCharacter);
	}
	else
	{
		OnLeaveInventory();
	}
}

void ACSWeapon::OnRep_Owner()
{
	SetCachedCharacter(GetOwner());
}
