// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "CSTablePaserCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSTablePaserCommandlet : public UCommandlet
{
	GENERATED_BODY()

	virtual int32 Main(const FString& Params) override;
	
};
