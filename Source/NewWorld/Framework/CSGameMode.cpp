// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameMode.h"
#include "GameFramework/GameSession.h"
#include "CSPlayerController.h"

namespace MatchState
{
	const FName PrepareMatchStart = FName(TEXT("PrepareMatchStart"));
}

ACSGameMode::ACSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}
// 
// bool ACSGameMode::IsMatchInProgress() const
// {
// 	if (GetMatchState() == MatchState::PrepareMatchStart)
// 	{
// 		return true;
// 	}
// 
// 	if (GetMatchState() == MatchState::InProgress)
// 	{
// 		return true;
// 	}
// 
// 	return false;
// }
// 
// void ACSGameMode::StartMatch()
// {
// 	if (HasMatchStarted())
// 	{
// 		// Already started
// 		return;
// 	}
// 
// 	//Let the game session override the StartMatch function, in case it wants to wait for arbitration
// 	if (GameSession->HandleStartMatchRequest())
// 	{
// 		return;
// 	}
// 
// 	SetMatchState(MatchState::InProgress);
// }
// 
// void ACSGameMode::OnMatchStateSet()
// {
// 	// DO NOT CALL Super
// 
// 	FGameModeEvents::OnGameModeMatchStateSetEvent().Broadcast(MatchState);
// 	// Call change callbacks
// 	if (MatchState == MatchState::WaitingToStart)
// 	{
// 		HandleMatchIsWaitingToStart();
// 	}
// 	else if (MatchState == MatchState::PrepareMatchStart)
// 	{
// 		
// 	}
// 	else if (MatchState == MatchState::InProgress)
// 	{
// 		HandleMatchHasStarted(); 
// 	}
// 	else if (MatchState == MatchState::WaitingPostMatch)
// 	{
// 		HandleMatchHasEnded();
// 	}
// 	else if (MatchState == MatchState::LeavingMap)
// 	{
// 		HandleLeavingMap();
// 	}
// 	else if (MatchState == MatchState::Aborted)
// 	{
// 		HandleMatchAborted();
// 	}
// }
// 
// void ACSGameMode::HandleMatchHasStarted()
// {
// 	Super::HandleMatchHasStarted();
// 
// 	//SetEndMatchStateOnNextTick(FTimerDelegate::CreateUObject(this, &ACSGameMode::OnEndPrepareMatchStart));
// }
// 
// void ACSGameMode::HandleMatchIsInProgress()
// {
// 	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
// 	{
// 		ACSPlayerController* PlayerController = Cast<ACSPlayerController>(Iterator->Get());
// 		if (PlayerController)
// 		{
// 			PlayerController->ClientMatchInProgress();
// 		}
// 	}
// }
// 
// void ACSGameMode::OnEndPrepareMatchStart()
// {
// 	SetMatchState(MatchState::InProgress);
// }
// 
// void ACSGameMode::SetEndMatchStateOnNextTick(const FTimerDelegate& TimerDelegate)
// {
// 	FTimerManager& TimerManager = GetWorldTimerManager();
// 	if (TimerHandle_OnEndMatchState.IsValid())
// 	{
// 		TimerManager.ClearTimer(TimerHandle_OnEndMatchState);
// 	}
// 
// 	TimerHandle_OnEndMatchState = TimerManager.SetTimerForNextTick(TimerDelegate);
// }
