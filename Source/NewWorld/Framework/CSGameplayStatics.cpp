// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameplayStatics.h"
#include "CSPlayerController.h"
#include <Blueprint/WidgetBlueprintLibrary.h>

UCSGameplayStatics::UCSGameplayStatics(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSGameplayStatics::SetInputMode(APlayerController* Target, UWidget* InWidgetToFocus, EInputMode InputMode)
{
	switch (InputMode)
	{
	case EInputMode::UIOnly:
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Target, InWidgetToFocus);
		break;
	case EInputMode::GameAndUI:
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Target, InWidgetToFocus, EMouseLockMode::LockOnCapture, true);
		break;
	case EInputMode::GameOnly:
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Target);
		break;
	default:
		break;
	}
}
