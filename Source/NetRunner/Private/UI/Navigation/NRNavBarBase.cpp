// Copyleft, Andrey Protsenko. No rights reserved.


#include "UI/Navigation/NRNavBarBase.h"

void UNRNavBarBase::AddNavigationElement(FText Header, bool bEnabled, FNRNavBarElementDelegate OnClicked)
{
	UNRNavBarButtonBase* pWidget = CreateWidget<UNRNavBarButtonBase>(this, NavBarButtonClass);
	FNRNavBarElementSetup OutSetup{};
	OutSetup.Header = Header;
	OutSetup.bEnabled = bEnabled;
	OutSetup.OnClicked = OnClicked;
	pWidget->OnClicked.BindUObject(this, &UNRNavBarBase::OnElementClicked);
	pWidget->UpdateProperties(Header, bEnabled);
	BoundButtons.Add(pWidget, MoveTemp(OutSetup));
	OnElementAdded(pWidget, Header, bEnabled);
}

void UNRNavBarBase::OnElementClicked(UNRNavBarButtonBase* Element)
{
	if (Element != nullptr && BoundButtons.Contains(Element))
	{
		BoundButtons[Element].OnClicked.ExecuteIfBound();
	}
}

void UNRNavBarButtonBase::PropagateClick()
{
	OnClicked.ExecuteIfBound(this);
}
