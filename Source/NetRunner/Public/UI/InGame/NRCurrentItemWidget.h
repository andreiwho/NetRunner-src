// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/NRWidget.h"
#include "NRCurrentItemWidget.generated.h"

class UTextBlock;
class UImage;
class ANRUsableItemBase;

UCLASS()
class NETRUNNER_API UNRCurrentItemWidget : public UNRWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetItem(TSubclassOf<ANRUsableItemBase> ItemClass);
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ItemText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;
};
