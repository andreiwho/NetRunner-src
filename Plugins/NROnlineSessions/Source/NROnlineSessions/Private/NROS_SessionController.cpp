// Copyleft, Andrey Protsenko. No rights reserved.


#include "NROS_SessionController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogSessionController, Log, All);

UNROS_SessionController::UNROS_SessionController(const FObjectInitializer& OI)
	: Super(OI)
	, OnCreateSessionCompleted_Delegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNROS_SessionController::OnCreateSessionCompleted))
	, OnDestroySessionCompleted_Delegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNROS_SessionController::OnDestroySessionCompleted))
	, OnFindSessionsCompleted_Delegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNROS_SessionController::OnFindSessionsCompleted))
	, OnJoinSessionCompleted_Delegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNROS_SessionController::OnJoinSessionCompleted))
{
	IOnlineSubsystem* pOSS = IOnlineSubsystem::Get();
	if (ensure(pOSS))
	{
		OnlineSessions = pOSS->GetSessionInterface();
		check(OnlineSessions);
	}
}

void UNROS_SessionController::CreateSession(const FNROS_HostSessionParameters& InParams)
{
	check(OnlineSessions);
	CurrentSessionParams = InParams;

	if (OnlineSessions->GetNamedSession(NAME_GameSession))
	{
		DestroyCurrentSession(true);
		return;
	}

	SavedSessionSettings = MakeShareable(new FOnlineSessionSettings());
	SavedSessionSettings->NumPublicConnections = CurrentSessionParams.MaxPlayersInGame;
	SavedSessionSettings->NumPrivateConnections = CurrentSessionParams.MaxPlayersInGame;
	SavedSessionSettings->bShouldAdvertise = true;
	SavedSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SavedSessionSettings->bAllowInvites = true;
	SavedSessionSettings->bAllowJoinInProgress = true;
	SavedSessionSettings->bUsesPresence = true;
	SavedSessionSettings->bAllowJoinViaPresence = true;
	SavedSessionSettings->bAntiCheatProtected = false;
	SavedSessionSettings->bUseLobbiesIfAvailable = true;
	SavedSessionSettings->Set(FName("GAMENAME"), CurrentSessionParams.GameName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
	SavedSessionSettings->Set(FName("GameProjectName"), FString("NetRunnerGame"), EOnlineDataAdvertisementType::ViaOnlineService);

	OnCreateSessionCompleted_Handle = OnlineSessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleted_Delegate);
	OnlineSessions->CreateSession(0, NAME_GameSession, *SavedSessionSettings);

	UE_LOG(LogSessionController, Display, TEXT("Creating session with game name: '%s', max players: '%d'"), *CurrentSessionParams.GameName.ToString(), CurrentSessionParams.MaxPlayersInGame);
}

void UNROS_SessionController::DestroyCurrentSession(bool bRecreateOnDestroy)
{
	check(OnlineSessions);

	// If we haven't created the session, then we won't destroy the session
	if (!SavedSessionSettings)
	{
		Public_OnSessionDestroyed.ExecuteIfBound(true);
		return;
	}
	
	SavedSessionSettings.Reset();
	bShouldCreateSessionAfterDestroy = bRecreateOnDestroy;

	OnDestroySessionCompleted_Handle = OnlineSessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleted_Delegate);
	OnlineSessions->DestroySession(NAME_GameSession);

	UE_LOG(LogSessionController, Display, TEXT("Destroying current session"));

}

void UNROS_SessionController::FindSessions()
{
	check(OnlineSessions);
	SearchResults.Empty();
	SavedSessionSettings.Reset();

	FindSessionResult = MakeShareable(new FOnlineSessionSearch);
	if (FindSessionResult.IsValid())
	{
		FindSessionResult->MaxSearchResults = SC_MaxSearchResults;
		FindSessionResult->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		FindSessionResult->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		FindSessionResult->QuerySettings.Set(FName("GameProjectName"), FString("NetRunnerGame"), EOnlineComparisonOp::Equals);
	}
	else
	{
		Public_OnSessionsFound.ExecuteIfBound(false);
		return;
	}

	OnFindSessionsCompleted_Handle = OnlineSessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleted_Delegate);
	OnlineSessions->FindSessions(0, FindSessionResult.ToSharedRef());
}

void UNROS_SessionController::JoinSession(const FNROS_SessionSearchResult& SearchResult)
{
	check(OnlineSessions);

	OnJoinSessionCompleted_Handle = OnlineSessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleted_Delegate);
	OnlineSessions->JoinSession(0, NAME_GameSession, SearchResult.SearchResult);
}

FString UNROS_SessionController::GetSessionConnectString(const FNROS_SessionSearchResult& SearchResult)
{
	check(OnlineSessions);

	FString OutConnectString;
	if (!OnlineSessions->GetResolvedConnectString(SearchResult.SearchResult, NAME_GamePort, OutConnectString))
	{
		UE_LOG(LogSessionController, Error, TEXT("Failed to get session connect string. Invalid session search result passed."));
		return {};
	}
	return OutConnectString;
}

void UNROS_SessionController::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	check(OnlineSessions);
	OnlineSessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleted_Handle);

	if (bWasSuccessful)
	{
		UE_LOG(LogSessionController, Display, TEXT("Online session successfully created with name: %s, max players %d"), *CurrentSessionParams.GameName.ToString(), CurrentSessionParams.MaxPlayersInGame);
	}

	Public_OnSessionCreated.ExecuteIfBound(bWasSuccessful);
}

void UNROS_SessionController::OnDestroySessionCompleted(FName Name, bool bWasSuccessful)
{
	check(OnlineSessions);
	OnlineSessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleted_Handle);

	if (bWasSuccessful)
	{
		UE_LOG(LogSessionController, Display, TEXT("Online session destroyed. Should recreate: %d"), (int32)bShouldCreateSessionAfterDestroy);
		if (bShouldCreateSessionAfterDestroy)
		{
			bShouldCreateSessionAfterDestroy = false;
			CreateSession(CurrentSessionParams);
		}
	}

	Public_OnSessionDestroyed.ExecuteIfBound(bWasSuccessful);
}

void UNROS_SessionController::OnFindSessionsCompleted(bool bWasSuccessful)
{
	OnlineSessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleted_Handle);
	if (bWasSuccessful && FindSessionResult.IsValid() && FindSessionResult->SearchResults.Num() > 0)
	{
		UE_LOG(LogSessionController, Display, TEXT("Found %d sessions."), FindSessionResult->SearchResults.Num());
		RefillSearchResults();
		Public_OnSessionsFound.ExecuteIfBound(true);
	}
	else
	{
		Public_OnSessionsFound.ExecuteIfBound(false);
	}
}

void UNROS_SessionController::RefillSearchResults()
{
	if (!FindSessionResult.IsValid())
	{
		return;
	}

	for (const FOnlineSessionSearchResult& SearchResult : FindSessionResult->SearchResults)
	{
		if (SearchResult.IsValid())
		{
			FNROS_SessionSearchResult OutInfo{};
			FString GameName;
			if (SearchResult.Session.SessionSettings.Get("GAMENAME", GameName))
			{
				OutInfo.GameName = FName(*GameName);
				OutInfo.MaxPlayersInGame = SearchResult.Session.SessionSettings.NumPublicConnections;
				OutInfo.SearchResult = SearchResult;
				SearchResults.Add(MoveTemp(OutInfo));
			}
		}
	}
}

void UNROS_SessionController::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	check(OnlineSessions);
	OnlineSessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleted_Handle);

	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
		UE_LOG(LogSessionController, Display, TEXT("Session joined successfully!"));
		Public_OnSessionJoined.ExecuteIfBound(true);
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogSessionController, Display, TEXT("Failed to join session: Session is full"));
		Public_OnSessionJoined.ExecuteIfBound(false);
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogSessionController, Display, TEXT("Failed to join session: Session does not exist"));
		Public_OnSessionJoined.ExecuteIfBound(false);
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogSessionController, Display, TEXT("Failed to join session: Could not retreive address"));
		Public_OnSessionJoined.ExecuteIfBound(false);
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogSessionController, Display, TEXT("Failed to join session: Already in session"));
		Public_OnSessionJoined.ExecuteIfBound(false);
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogSessionController, Display, TEXT("Failed to join session: Unknown error."));
		Public_OnSessionJoined.ExecuteIfBound(false);
		break;
	}
}
