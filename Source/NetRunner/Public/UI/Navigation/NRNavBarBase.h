// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/NRWidget.h"
#include "NRNavBarBase.generated.h"

DECLARE_DYNAMIC_DELEGATE(FNRNavBarElementDelegate);

USTRUCT(BlueprintType)
struct FNRNavBarElementSetup
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Header;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled;

	UPROPERTY()
	FNRNavBarElementDelegate OnClicked;
};

DECLARE_DELEGATE_OneParam(FNRNavBar_ButtonClicked, class UNRNavBarButtonBase*);

UCLASS()
class NETRUNNER_API UNRNavBarButtonBase : public UNRWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProperties(const FText& Text, bool bInitialEnabled);

	/**
	 * Child widget must call this callback in order for the nav bar to receive the message
	 */
	UFUNCTION(BlueprintCallable)
	void PropagateClick();

	FNRNavBar_ButtonClicked OnClicked;
};

UCLASS()
class NETRUNNER_API UNRNavBarBase : public UNRWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Navigation")
	TSubclassOf<UNRNavBarButtonBase> NavBarButtonClass;

	UFUNCTION(BlueprintCallable, Category="Navigation")
	void AddNavigationElement(FText Header, bool bEnabled, FNRNavBarElementDelegate OnClick);

	UFUNCTION(BlueprintImplementableEvent)
	void OnElementAdded(UNRNavBarButtonBase* NewElement, const FText& Label, bool bInitialEnabled);

private:
	void OnElementClicked(UNRNavBarButtonBase* Element);

private:
	UPROPERTY()
	TMap<UNRNavBarButtonBase*, FNRNavBarElementSetup> BoundButtons; 
};
