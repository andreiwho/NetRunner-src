// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/NRWidget.h"
#include "NRPlaceWidget.generated.h"

class UTextBlock;

UCLASS()
class NETRUNNER_API UNRPlaceWidget : public UNRWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerPlace(int32 Place, int32 TotalPlayerCount);

	UFUNCTION(BlueprintCAllable)
	void SetPlayerLap(int32 Lap);

	UPROPERTY(BlueprintreadWrite, meta=(BindWidget))
	UTextBlock* LapText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* PlaceText;
};
