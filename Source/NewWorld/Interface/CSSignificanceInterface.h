// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <SignificanceManager.h>


class NEWWORLD_API FCSSignificanceHandler
{
public:
	FCSSignificanceHandler() : Temp(0) {}
	virtual ~FCSSignificanceHandler() {}

protected:
	float GetSignificanceByDistance(UObject* InObject, const FTransform& ViewTransform) const;

protected:
	int32 Temp;
};

class NEWWORLD_API ICSSignificanceInterface : public FCSSignificanceHandler
{		
public:
	void RegisterObject(UObject* InObject);
	void UnRegisterObject(UObject* InObject);

	virtual float SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewTransform) = 0;
	virtual void PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal) = 0;
	virtual FName GetObjectTag() const = 0;
};