// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CSGameState.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void OnRep_MatchState() override;
	virtual void HandleMatchIsInProgress();

	DECLARE_EVENT_TwoParams(ACSGameState, FOnGameStateChanged, const FName&, const FName&)
	FOnGameStateChanged& OnGameStateChanged() { return GameStateChangeEvent; }

protected:
	FOnGameStateChanged GameStateChangeEvent;
};
