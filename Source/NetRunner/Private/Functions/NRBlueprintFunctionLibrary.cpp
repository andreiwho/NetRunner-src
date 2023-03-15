// Copyleft, Andrey Protsenko. No rights reserved.

#include "Functions/NRBlueprintFunctionLibrary.h"
#include "Misc/DefaultValueHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogNR, Log, All);

void UNRBlueprintFunctionLibrary::NR_Log(UObject* WorldContextObject, ENRPrintLogVerbosity Verbosity, bool bShouldPrintToLog, const FString& Category, const FString& Message)
{
	if (bShouldPrintToLog)
	{
		switch (Verbosity)
		{
		case ENRPrintLogVerbosity::Display:
			UE_LOG(LogNR, Display, TEXT("%s: %s"), *Category, *Message);
			break;
		case ENRPrintLogVerbosity::Warning:
			UE_LOG(LogNR, Warning, TEXT("%s: %s"), *Category, *Message);
			break;
		case ENRPrintLogVerbosity::Error:
			UE_LOG(LogNR, Error, TEXT("%s: %s"), *Category, *Message);
			break;
		default:
			break;
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, Invoke(
			[Verbosity]
			{
				switch (Verbosity)
				{
				case ENRPrintLogVerbosity::Display:
					return FColor::Green;
				case ENRPrintLogVerbosity::Warning:
					return FColor::Yellow;
				case ENRPrintLogVerbosity::Error:
					return FColor::Red;
				default:
					break;
				}
				return FColor::Magenta;
			}), 
			FString::Printf(TEXT("%s: %s"), *Category, *Message));
	}
}

int32 UNRBlueprintFunctionLibrary::StringToInt(const FString& String, bool& bOutIsValid)
{
	int32 OutValue = -1;
	bOutIsValid = FDefaultValueHelper::ParseInt(String, OutValue);
	return OutValue;
}

void UNRBlueprintFunctionLibrary::NR_ClientTravel(UObject* WorldContextObject, const FString& ConnectString)
{
	if (UWorld* pWorld = WorldContextObject->GetWorld())
	{
		if (APlayerController* pPlayerController = pWorld->GetFirstPlayerController())
		{
			pPlayerController->ClientTravel(ConnectString, TRAVEL_Absolute, false);	// TODO: Seamless travel
		}
	}
}
