// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NROS_PublicTypes.h"
#include "OnlineSessionSettings.h"
#include "NROS_SessionController.generated.h"

DECLARE_DELEGATE_OneParam(FNROS_OnSessionCreatedCallback, bool);
DECLARE_DELEGATE_OneParam(FNROS_OnSessionDestroyedCallback, bool);
DECLARE_DELEGATE_OneParam(FNROS_OnSessionsFoundCallback, bool);
DECLARE_DELEGATE_OneParam(FNROS_OnSessionsJoinedCallback, bool);

USTRUCT(BlueprintType)
struct FNROS_HostSessionParameters
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NetRunner|OnlineSessions")
	FName GameName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NetRunner|OnlineSessions")
	int32 MaxPlayersInGame;
};

UCLASS()
class NRONLINESESSIONS_API UNROS_SessionController : public UObject
{
	GENERATED_BODY()

public:
	UNROS_SessionController(const FObjectInitializer& OI);

	void CreateSession(const FNROS_HostSessionParameters& InParams);
	void DestroyCurrentSession(bool bRecreateOnDestroy = false);
	void FindSessions();
	void JoinSession(const FNROS_SessionSearchResult& SearchResult);
	FString GetSessionConnectString(const FNROS_SessionSearchResult& SearchResult);

	FNROS_OnSessionCreatedCallback Public_OnSessionCreated;
	FNROS_OnSessionDestroyedCallback Public_OnSessionDestroyed;
	FNROS_OnSessionsFoundCallback Public_OnSessionsFound;
	FNROS_OnSessionsJoinedCallback Public_OnSessionJoined;

	FORCEINLINE const TArray<FNROS_SessionSearchResult>& GetSessionSearchResults() const
	{
		return SearchResults;
	}

private:
	static constexpr int32 SC_MaxSearchResults = 1000;

	IOnlineSessionPtr OnlineSessions{};

	FNROS_HostSessionParameters CurrentSessionParams{};

	TSharedPtr<FOnlineSessionSettings> SavedSessionSettings{};

	/**
	 * Create session callback
	 */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleted_Delegate;
	FDelegateHandle OnCreateSessionCompleted_Handle;
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);

	/**
	 * Destroy session callback
	 */
	 FOnDestroySessionCompleteDelegate OnDestroySessionCompleted_Delegate;
	 FDelegateHandle OnDestroySessionCompleted_Handle;
	 void OnDestroySessionCompleted(FName Name, bool bWasSuccessful);
	 bool bShouldCreateSessionAfterDestroy = false;

	 /**
	  * Find sessions callbacks
	  */
	  FOnFindSessionsCompleteDelegate OnFindSessionsCompleted_Delegate;
	  FDelegateHandle OnFindSessionsCompleted_Handle;
	  void OnFindSessionsCompleted(bool bWasSuccessful);
	  TSharedPtr<FOnlineSessionSearch> FindSessionResult;
	  TArray<FNROS_SessionSearchResult> SearchResults;
	  void RefillSearchResults();

	  /**
	   * Join session callback
	   */
	   FOnJoinSessionCompleteDelegate OnJoinSessionCompleted_Delegate;
	   FDelegateHandle OnJoinSessionCompleted_Handle;
	   void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
