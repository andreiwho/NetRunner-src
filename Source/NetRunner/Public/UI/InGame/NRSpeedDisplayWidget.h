// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/NRWidget.h"
#include "NRSpeedDisplayWidget.generated.h"

class UTextBlock;

UCLASS()
class NETRUNNER_API UNRSpeedDisplayWidget : public UNRWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSpeedModifier(float Modifier);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PostSetSpeedModifier(float Speed);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* SpeedText;
};
