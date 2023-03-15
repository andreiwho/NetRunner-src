// Copyleft, Andrey Protsenko. No rights reserved.

#include "UI/InGame/NRCurrentItemWidget.h"
#include "Game/NRUsableItemBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UNRCurrentItemWidget::SetItem(TSubclassOf<ANRUsableItemBase> ItemClass)
{
	ANRUsableItemBase* Item = ItemClass ? ItemClass->GetDefaultObject<ANRUsableItemBase>() : nullptr;
	if (Item)
	{
		ItemText->SetText(FText::FromString(Item->ItemName));
		if (Item->ItemIcon)
		{
			ItemImage->SetBrushFromTexture(Item->ItemIcon);
			ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		ItemText->SetText(FText::FromString("None"));
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
