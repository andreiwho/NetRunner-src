// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NRLapSpline.generated.h"

class USplineComponent;

UCLASS()
class NETRUNNER_API ANRLapSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ANRLapSpline();

	UPROPERTY(EditAnywhere)
	USplineComponent* LapSpline;

	float GetSplinePositionGradient(const FVector& PlayerPosition) const;
};
