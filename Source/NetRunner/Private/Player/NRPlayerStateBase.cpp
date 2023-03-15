// Copyleft, Andrey Protsenko. No rights reserved.


#include "Player/NRPlayerStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/NRLapSpline.h"
#include "Functions/NRBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/NRGameMode_Common.h"

void ANRPlayerStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ANRPlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANRPlayerStateBase, CurrentLap);
	DOREPLIFETIME(ANRPlayerStateBase, LapPosition);
	DOREPLIFETIME(ANRPlayerStateBase, PlayerPlace);
}

void ANRPlayerStateBase::IncrementLap()
{
	if (!CanIncrementLap())
	{
		return;
	}

	CurrentLap++;
	ReceiveCurrentLap();
	
	if (UWorld* pWorld = GetWorld())
	{
		if (ANRGameMode_Common* pGameMode = pWorld->GetAuthGameMode<ANRGameMode_Common>())
		{
			if (CurrentLap > pGameMode->LapCount)
			{
				pGameMode->PlayerFinished(this);
			}
		}
	}
}

void ANRPlayerStateBase::UpdateLapPosition(float NewPosition)
{
	LapPosition = NewPosition + (float)CurrentLap;
}

void ANRPlayerStateBase::OnRep_CurrentLap()
{
	ReceiveCurrentLap();
}

void ANRPlayerStateBase::ReceiveCurrentLap()
{
	UNRBlueprintFunctionLibrary::NR_Log(GetWorld(), ENRPrintLogVerbosity::Display, true, "Default", FString::Printf(TEXT("New player lap: %d"), CurrentLap));
}

