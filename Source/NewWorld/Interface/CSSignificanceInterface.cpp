// Fill out your copyright notice in the Description page of Project Settings.


#include "CSSignificanceInterface.h"

float FCSSignificanceHandler::GetSignificanceByDistance(UObject* InObject, const FTransform& ViewTransform) const
{
	return 0.f;
}

void ICSSignificanceInterface::RegisterObject(UObject* InObject)
{
	if (!IsValid(InObject))
	{
		return;
	}

	USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(InObject->GetWorld());
	if (SignificanceManager)
	{
		auto SignificanceLambda = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewTransform) -> float
		{
			return SignificanceFunction(ObjectInfo, ViewTransform);
		};

		auto PostSignificanceLambda = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
		{
			return PostSignificanceFunction(ObjectInfo, OldSignificance, Significance, bFinal);
		};

		SignificanceManager->RegisterObject(InObject, GetObjectTag(), SignificanceLambda, USignificanceManager::EPostSignificanceType::Sequential, PostSignificanceLambda);
	}
}

void ICSSignificanceInterface::UnRegisterObject(UObject* InObject)
{
	if (!IsValid(InObject))
	{
		return;
	}

	USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(InObject->GetWorld());
	if (SignificanceManager)
	{
		SignificanceManager->UnregisterObject(InObject);
	}
}
