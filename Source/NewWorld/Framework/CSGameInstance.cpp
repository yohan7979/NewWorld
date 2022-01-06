// copyrighted by swonlee ... twiclok@naver.com


#include "CSGameInstance.h"
#include "CSGameSingleton.h"
#include "CSDataTableManager.h"

UCSGameInstance::UCSGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameSingleton = nullptr;
}

void UCSGameInstance::Init()
{
	Super::Init();

	if (GameSingleton)
	{
		GameSingleton->Init(this);
	}
}

void UCSGameInstance::Shutdown()
{
	Super::Shutdown();

	if (GameSingleton)
	{
		GameSingleton->Shutdown();
	}
}

void UCSGameInstance::PreloadContentForURL(FURL InURL)
{
	Super::PreloadContentForURL(InURL);

	if (GameSingleton)
	{
		GameSingleton->PreloadContentForURL(InURL);
	}
}
