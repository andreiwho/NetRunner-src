// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NRUsableItemBase.generated.h"

class UTexture2D;
class ANRPlayerControllerBase;

UCLASS()
class NETRUNNER_API ANRUsableItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ANRUsableItemBase();

	UPROPERTY(EditAnywhere, Category="Item")
	FString ItemName;

	UPROPERTY(EditANywhere, Category="Item")
	UTexture2D* ItemIcon;
};
