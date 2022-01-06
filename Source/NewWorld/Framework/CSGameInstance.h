// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CSGameInstance.generated.h"

class UCSDataTableManager;

UCLASS()
class NEWWORLD_API UCSGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

public:
	//~ Begin UGameInstance interface
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void PreloadContentForURL(FURL InURL) override;
	//~ End UGameInstance  interface

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCSDataTableManager> DataTableManager;

private:
	class UCSGameSingleton* GameSingleton;
};
