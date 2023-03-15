// Copyleft, Andrey Protsenko. No rights reserved.


#include "UI/InGame/NRPlaceWidget.h"
#include "Components/TextBlock.h"

void UNRPlaceWidget::SetPlayerPlace(int32 Place, int32 TotalPlayerCount)
{
	FText Text = FText::FromString(FString::Printf(TEXT("%d / %d"), Place + 1, TotalPlayerCount));
	PlaceText->SetText(Text);
}

void UNRPlaceWidget::SetPlayerLap(int32 Lap)
{
	LapText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Lap)));
}
