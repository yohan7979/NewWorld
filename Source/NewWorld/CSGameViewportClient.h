// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "CSGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSGameViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Tick( float DeltaTime ) override;

protected:
	TWeakObjectPtr<class APlayerController> CachedPlayerController;
};
