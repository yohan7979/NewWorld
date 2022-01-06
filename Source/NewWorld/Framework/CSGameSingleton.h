// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSGameSingleton.generated.h"

class UCSGameInstance;

UCLASS()
class NEWWORLD_API UCSGameSingleton : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	static UCSGameSingleton* Get()
	{
		return CastChecked<UCSGameSingleton>(GEngine->GameSingleton);
	}

	virtual void Init(UCSGameInstance* InGameInstance);
	virtual void Shutdown();
	virtual void PreloadContentForURL(FURL InURL);

private:
	UPROPERTY(Transient)
	UCSGameInstance* GameInstance;
};
