// Copyleft, Andrey Protsenko. No rights reserved.


#include "UI/InGame/NRSpeedDisplayWidget.h"
#include "Components/TextBlock.h"

void UNRSpeedDisplayWidget::UpdateSpeedModifier(float Modifier)
{
	FString Text = FString::Printf(TEXT("%.1f"), Modifier);
	SpeedText->SetText(FText::FromString(Text));

	PostSetSpeedModifier(Modifier);
}
