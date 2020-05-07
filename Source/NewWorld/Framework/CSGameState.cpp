// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameState.h"
#include "CSGameMode.h"

ACSGameState::ACSGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ACSGameState::OnRep_MatchState()
{
	if (MatchState == MatchState::WaitingToStart || PreviousMatchState == MatchState::EnteringMap)
	{
		// Call MatchIsWaiting to start even if you join in progress at a later state
		HandleMatchIsWaitingToStart();
	}

	if (MatchState == MatchState::PrepareMatchStart)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchIsInProgress();
	}
	else if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
	else if (MatchState == MatchState::LeavingMap)
	{
		HandleLeavingMap();
	}

	if (GameStateChangeEvent.IsBound())
	{
		GameStateChangeEvent.Broadcast(PreviousMatchState, MatchState);
	}

	PreviousMatchState = MatchState;
}

void ACSGameState::HandleMatchIsInProgress()
{

}
