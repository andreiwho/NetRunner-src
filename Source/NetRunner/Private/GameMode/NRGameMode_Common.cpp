// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NRGameMode_Common.h"
#include "Controller/NRPlayerControllerBase.h"
#include "Game/NRCheckPointBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Game/NRLapSpline.h"
#include "Kismet/GameplayStatics.h"
#include "Functions/NRBlueprintFunctionLibrary.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Player/NRPlayerStateBase.h"

ANRGameMode_Common::ANRGameMode_Common()
{
	PlayerStateClass = ANRPlayerStateBase::StaticClass();
	bUseSeamlessTravel = true;
}

void ANRGameMode_Common::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	if (UWorld* pWorld = GetWorld())
	{
		LapSplineActor = Cast<ANRLapSpline>(UGameplayStatics::GetActorOfClass(pWorld, ANRLapSpline::StaticClass()));
		if (!LapSplineActor)
		{
			UNRBlueprintFunctionLibrary::NR_Log(pWorld, ENRPrintLogVerbosity::Error, true, "GameMode", "Failed to find lap spline actor on the map.");
		}
	}
	NextMap = MapType::RandomMap();
}

void ANRGameMode_Common::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MatchState == MatchState::InProgress)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_GameModeCommon_CollectMapInfo);

		CollectPlayerLapInfo();
		CalculatePlayerPlaces();
	}
}

void ANRGameMode_Common::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ANRPlayerStateBase* pPlayerState = NewPlayer->GetPlayerState<ANRPlayerStateBase>())
	{
		InGamePlaces.AddUnique(pPlayerState);
	}
}

void ANRGameMode_Common::TeleportPlayerToLastCheckpoint(ANRPlayerControllerBase* pPlayerController)
{
	if (!pPlayerController)
	{
		return;
	}

	if (!pPlayerController->LastCheckpoint)
	{
		APawn* pPawn = pPlayerController->GetPawn();
		AActor* pStart = FindPlayerStart(pPlayerController);
		pPawn->TeleportTo(pStart->GetActorLocation(), pStart->GetActorRotation());
		return;
	}

	APawn* pPawn = pPlayerController->GetPawn();
	const FTransform TeleportTransform = pPlayerController->LastCheckpoint->GetRestartTransform();
	if (!pPawn->TeleportTo(TeleportTransform.GetLocation(), TeleportTransform.GetRotation().Rotator()))
	{
		AActor* pStart = FindPlayerStart(pPlayerController);
		pPawn->TeleportTo(pStart->GetActorLocation(), pStart->GetActorRotation());
	}
}

void ANRGameMode_Common::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	// Propagate game mode to all player controllers
	if (UWorld* pWorld = GetWorld())
	{
		for (FConstPlayerControllerIterator It = pWorld->GetPlayerControllerIterator(); It; ++It)
		{
			if (ANRPlayerControllerBase* pPlayerController = Cast<ANRPlayerControllerBase>(*It))
			{
				pPlayerController->OnMatchStateSet(GetMatchState());
			}
		}
	}
}

void ANRGameMode_Common::PlayerFinished(APlayerState* PlayerState)
{
	if (!GetWorld())
	{
		return;
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ANRPlayerControllerBase* pPlayerController = Cast<ANRPlayerControllerBase>(*It))
		{
			pPlayerController->PlayerFinished(PlayerState);
		}
	}
	FinishedPlayerCount++;

	if (FinishedPlayerCount >= GameState->PlayerArray.Num())
	{
		// Transition to next map
		if (UWorld* pWorld = GetWorld())
		{
			if (!pWorld->GetTimerManager().IsTimerActive(TravelTimer))
			{
				pWorld->GetTimerManager().SetTimer(TravelTimer, this, &ANRGameMode_Common::OnTravelTimerFinished, 3.0f);
				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
				{
					if (ANRPlayerControllerBase* pPlayerController = Cast<ANRPlayerControllerBase>(*It))
					{
						pPlayerController->ReceiveWaitingNewMap();
					}
				}
			}
		}
	}
}

void ANRGameMode_Common::OnTravelTimerFinished()
{
	UWorld* pWorld = GetWorld();
	if (!pWorld)
	{
		return;
	}

	UNRBlueprintFunctionLibrary::NR_Log(this, ENRPrintLogVerbosity::Display, true, "Default", "Traveling...");
	if (pWorld->ServerTravel(MapType::ToMapURL(NextMap), true))
	{
		UNRBlueprintFunctionLibrary::NR_Log(this, ENRPrintLogVerbosity::Display, true, "Default", "Travel success.");
	}
	else
	{
		UNRBlueprintFunctionLibrary::NR_Log(this, ENRPrintLogVerbosity::Error, true, "Default", "Travel failure.");
	}
}

void ANRGameMode_Common::CollectPlayerLapInfo()
{
	if (!LapSplineActor)
	{
		return;
	}

	if (AGameState* pGameState = GetGameState<AGameState>())
	{
		for (APlayerState* pPlayer : pGameState->PlayerArray)
		{
			ANRPlayerStateBase* pNRPlayerState = Cast<ANRPlayerStateBase>(pPlayer);
			if (!pNRPlayerState)
			{
				continue;
			}

			if (APawn* pPawn = pNRPlayerState->GetPawn())
			{
				pNRPlayerState->UpdateLapPosition(LapSplineActor->GetSplinePositionGradient(pPawn->GetActorLocation()));
			}
		}
	}
}

void ANRGameMode_Common::CalculatePlayerPlaces()
{
	if (!LapSplineActor)
	{
		return;
	}

	InGamePlaces.Sort([](const ANRPlayerStateBase& pLHS, const ANRPlayerStateBase& pRHS)
		{
			return pLHS.LapPosition > pRHS.LapPosition;
		});

	for (ANRPlayerStateBase* pState : InGamePlaces)
	{
		pState->PlayerPlace = InGamePlaces.IndexOfByKey(pState);
	}
}
