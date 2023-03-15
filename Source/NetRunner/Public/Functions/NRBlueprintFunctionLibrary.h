// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NROS_PublicTypes.h"
#include "NRBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ENRPrintLogVerbosity : uint8
{
	Display,
	Warning,
	Error,
};

UCLASS()
class NETRUNNER_API UNRBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void NR_Log(UObject* WorldContextObject, ENRPrintLogVerbosity Verbosity, bool bShouldPrintToLog = true, const FString& Category = "Default", const FString& Message = "");

	// Utility
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 StringToInt(const FString& String, bool& bOutIsValid);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void NR_ClientTravel(UObject* WorldContextObject, const FString& ConnectString);
};
