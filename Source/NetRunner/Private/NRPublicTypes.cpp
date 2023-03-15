// Copyleft, Andrey Protsenko. No rights reserved.

#include "NRPublicTypes.h"

namespace MapType
{
	FString ToMapURL(ENRMapType Type)
	{
		switch (Type)
		{
		case ENRMapType::TestMap:
			return FString("/Game/Maps/L_Test");
		/*case ENRMapType::TheDash:
			return FString("Game/Maps/L_TheDash");
		*/
		}
		return {};
	}

	ENRMapType RandomMap()
	{
		return static_cast<ENRMapType>(FMath::RandRange(0, (int32)ENRMapType::MT_MAX - 1));
	}
}