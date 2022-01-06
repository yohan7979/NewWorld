// copyrighted by swonlee ... twiclok@naver.com


#include "CSGameViewportClient.h"
#include "SignificanceManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UCSGameViewportClient::UCSGameViewportClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSGameViewportClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* const pWorld = GetWorld();
	if (pWorld)
	{
		if (!CachedPlayerController.IsValid())
		{			
			CachedPlayerController = UGameplayStatics::GetPlayerController(pWorld, 0);
		}
		
		if (CachedPlayerController.IsValid())
		{
			USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(pWorld);
			if (SignificanceManager)
			{
				FVector ViewLocation;
				FRotator ViewRotation;
				CachedPlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

				TArray<FTransform> ViewTransform { FTransform(ViewRotation, ViewLocation) };				
				SignificanceManager->Update(ViewTransform);
			}
		}
	}
	
}
