// Fill out your copyright notice in the Description page of Project Settings.


#include "CSGameMode.h"
#include "GameFramework/GameSession.h"
#include "CSPlayerController.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "CSGameState.h"

namespace MatchState
{
	const FName PrepareMatchStart = FName(TEXT("PrepareMatchStart"));
}

ACSGameMode::ACSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ACSGameState::StaticClass();
}

void ACSGameMode::StartMatch()
{
	if (HasMatchStarted())
	{
		// Already started
		return;
	}

	//Let the game session override the StartMatch function, in case it wants to wait for arbitration
	if (GameSession->HandleStartMatchRequest())
	{
		return;
	}

	SetMatchState(MatchState::PrepareMatchStart);
}

bool ACSGameMode::ReadyToStartMatch_Implementation()
{
	if (GetWorld()->IsPlayInEditor())
	{
		return true;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		int32 NumPlayersWhomReady = 0;
		APlayerController* PC = Iterator->Get();
		if (PC && PC->HasClientLoadedCurrentWorld())
		{
			++NumPlayersWhomReady;
			if (NumPlayersWhomReady >= 1) // min num player is 1 (temporal)
			{
				return true;
			}
		}
	}

	return false;
}

void ACSGameMode::OnMatchStateSet()
{
	// DO NOT CALL SUPER

	FGameModeEvents::OnGameModeMatchStateSetEvent().Broadcast(MatchState);

	// Call change callbacks
	if (MatchState == MatchState::WaitingToStart)
	{
		HandleMatchIsWaitingToStart();
	}
	else if (MatchState == MatchState::PrepareMatchStart)
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
	else if (MatchState == MatchState::Aborted)
	{
		HandleMatchAborted();
	}
}

void ACSGameMode::HandleMatchHasStarted()
{
	GameSession->HandleMatchHasStarted();

	// Make sure level streaming is up to date before triggering NotifyMatchStarted
	GEngine->BlockTillLevelStreamingCompleted(GetWorld());
	
	// First fire BeginPlay, if we haven't already in waiting to start match
	GetWorldSettings()->NotifyBeginPlay();

	// Then fire off match started
	GetWorldSettings()->NotifyMatchStarted();

	if (IsHandlingReplays())
	{
		UGameInstance* GameInstance = GetGameInstance();
		if (GameInstance)
		{
			GameInstance->StartRecordingReplay(GetWorld()->GetMapName(), GetWorld()->GetMapName());
		}
	}

	SetEndMatchStateOnNextTick(FTimerDelegate::CreateUObject(this, &ACSGameMode::OnEndPrepareMatchStart));
}

void ACSGameMode::HandleMatchIsInProgress()
{
	NotifyAllPlayerControllers([&](ACSPlayerController* PlayerController)
	{
		RestartPlayer(PlayerController);
		PlayerController->ClientMatchInProgress();
	});
}

void ACSGameMode::OnEndPrepareMatchStart()
{
	SetMatchState(MatchState::InProgress);
}

void ACSGameMode::SetEndMatchStateOnNextTick(const FTimerDelegate& TimerDelegate)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerHandle_OnEndMatchState.IsValid())
	{
		TimerManager.ClearTimer(TimerHandle_OnEndMatchState);
	}

	TimerHandle_OnEndMatchState = TimerManager.SetTimerForNextTick(TimerDelegate);
}

void ACSGameMode::NotifyAllPlayerControllers(TFunction<void(ACSPlayerController*)>&& NotifyFunc)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator() ; Iterator ; ++Iterator)
	{
		ACSPlayerController* PlayerController = Cast<ACSPlayerController>(Iterator->Get());
		if (PlayerController)
		{
			NotifyFunc(PlayerController);
		}
	}
}
