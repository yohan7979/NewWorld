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

	SkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetVisibility(false);
	SkeletalMesh->SetGenerateOverlapEvents(false);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetVisibility(false);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
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

}

void ACSWeapon::OnUnEquip()
{

}

void ACSWeapon::OnEnterInventory(ACSCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void ACSWeapon::OnLeaveInventory()
{
	SetOwningPawn(nullptr);
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
}

void ACSWeapon::OnRep_Owner()
{
	SetCachedCharacter(GetOwner());
}
