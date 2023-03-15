// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NRPublicTypes.generated.h"

UENUM(BlueprintType)
enum class ENRMapType : uint8
{
	TestMap,
	//TheDash,
	MT_MAX UMETA(Hidden),
};

namespace MapType
{
	FString ToMapURL(ENRMapType Type);
	ENRMapType RandomMap();
}