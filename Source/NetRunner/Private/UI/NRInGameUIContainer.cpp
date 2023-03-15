// Copyleft, Andrey Protsenko. No rights reserved.


#include "UI/NRInGameUIContainer.h"
#include "Controller/NRPlayerControllerBase.h"
#include "UI/InGame/NRSpeedDisplayWidget.h"
#include "UI/InGame/NRPlaceWidget.h"

void UNRInGameUIContainer::OnMatchStateSet(FName NewState)
{
	// Call blueprint event
	ReceiveOnMatchStateSet(NewState);
}

void UNRInGameUIContainer::ReceiveOnOpenMenu_Implementation()
{
	bIsMenuOpened = true;
}

void UNRInGameUIContainer::ReceiveOnCloseMenu_Implementation()
{
	bIsMenuOpened = false;
}

void UNRInGameUIContainer::ReceiveUpdateLapInfo_Implementation(int32 PlayerPlace, int32 TotalPlayers, int32 Lap)
{
	if (LapDisplay)
	{
		LapDisplay->SetPlayerPlace(PlayerPlace, TotalPlayers);
		LapDisplay->SetPlayerLap(Lap);
	}
}

void UNRInGameUIContainer::UpdateSpeedModifier_Implementation(float Modifier)
{
	if (SpeedDisplay)
	{
		SpeedDisplay->UpdateSpeedModifier(Modifier);
	}
}
