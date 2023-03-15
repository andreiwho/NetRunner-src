// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "NROS_PublicTypes.generated.h"

USTRUCT(BlueprintType)
struct FNROS_SessionSearchResult
{
	GENERATED_USTRUCT_BODY()
public:
	FOnlineSessionSearchResult SearchResult{};

	UPROPERTY(BlueprintReadOnly)
	FName GameName;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayersInGame;
};