// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NROS_GameInstance.generated.h"

class UNROS_SessionController;

UCLASS()
class NRONLINESESSIONS_API UNROS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="NetRunner|OnlineSessions")
	FORCEINLINE UNROS_SessionController* GetSessionController() const { return SessionController; }

private:
	UPROPERTY(Transient)
	UNROS_SessionController* SessionController;
};
