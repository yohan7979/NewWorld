// copyrighted by swonlee ... twiclok@naver.com


#include "CSGameSingleton.h"
#include "CSGameInstance.h"

UCSGameSingleton::UCSGameSingleton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameInstance = nullptr;
}

void UCSGameSingleton::Init(UCSGameInstance* InGameInstance)
{
	GameInstance = InGameInstance;

	
}

void UCSGameSingleton::Shutdown()
{
	GameInstance = nullptr;
}

void UCSGameSingleton::PreloadContentForURL(FURL InURL)
{

}
