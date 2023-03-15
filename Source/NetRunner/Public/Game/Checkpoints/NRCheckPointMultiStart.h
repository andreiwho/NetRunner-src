// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRCheckPointBase.h"
#include "NRCheckPointMultiStart.generated.h"

UCLASS()
class NETRUNNER_API ANRCheckPointMultiStart : public ANRCheckPointBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterComponentTransform(USceneComponent* Component);

	virtual FTransform GetRestartTransform() const override;

private:
	TArray<FTransform> Transforms;
	
};
