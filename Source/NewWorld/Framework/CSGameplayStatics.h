// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "CSTypes.h"
#include "CSGameplayStatics.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSGameplayStatics : public UGameplayStatics
{
	GENERATED_UCLASS_BODY()
	
public:
	template<typename T>
	static T* GetLoadedObject(TSoftObjectPtr<T>& SoftObjectPtr)
	{
		if (!SoftObjectPtr.IsValid())
		{
			return SoftObjectPtr.LoadSynchronous();
		}

		return SoftObjectPtr.Get();
	}

	template<typename T>
	static T* GetLoadedClass(TSoftClassPtr<T>& SoftClassPtr)
	{
		if (!SoftClassPtr.IsValid())
		{
			return SoftClassPtr.LoadSynchronous();
		}

		return SoftClassPtr.Get();
	}

	static void SetInputMode(APlayerController* Target, UWidget* InWidgetToFocus, EInputMode InputMode);
};
