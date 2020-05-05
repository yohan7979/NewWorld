// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CSTypes.h"
#include "CSGameMode.generated.h"

namespace MatchState
{
	extern NEWWORLD_API const FName PrepareMatchStart;
}

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
	
// 	virtual bool IsMatchInProgress() const override;
// 	virtual void StartMatch() override;
// 	
// protected:
// 	/** Overridable virtual function to dispatch the appropriate transition functions before GameState and Blueprints get SetMatchState calls. */
// 	virtual void OnMatchStateSet() override;
// 
// 	virtual void HandleMatchHasStarted() override;
// 	virtual void HandleMatchIsInProgress();
// 
// 	virtual void OnEndPrepareMatchStart();
// 
// 	void SetEndMatchStateOnNextTick(const FTimerDelegate& TimerDelegate);
// 	FTimerHandle TimerHandle_OnEndMatchState;
};
