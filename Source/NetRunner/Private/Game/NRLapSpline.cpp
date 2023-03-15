// Copyleft, Andrey Protsenko. No rights reserved.


#include "Game/NRLapSpline.h" 
#include "Components/SplineComponent.h"
#include "Functions/NRBlueprintFunctionLibrary.h"

ANRLapSpline::ANRLapSpline()
{
	LapSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LapSpline"));
	if (LapSpline)
	{
		SetRootComponent(LapSpline);
	}
}

float ANRLapSpline::GetSplinePositionGradient(const FVector& PlayerPosition) const
{
	return LapSpline->FindInputKeyClosestToWorldLocation(PlayerPosition) / (float)LapSpline->GetNumberOfSplinePoints();
}