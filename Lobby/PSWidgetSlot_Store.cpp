// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_Store.h"

#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"
#include "Components/Overlay.h"

void UPSWidgetSlot_Store::InitData()
{
	Super::InitData();

	if(!strGoodsType.IsEmpty())
	{
		m_eGoodsType = nGoodsType::GetEnum(strGoodsType,false);
	}

	SetSlotInfo();
}

void UPSWidgetSlot_Store::ReleaseData()
{
	Super::ReleaseData();
}

void UPSWidgetSlot_Store::TryPurchaseDia()
{
	if (g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->m_SelectedGoodsPrice = FCString::Atoi(*strGoodsPrice);
		g_PSCharacterMgr->m_SelectedGoodsNumber = GoodsNumber;
	}
	if(g_GameMsgMgrValid)
	{
		g_GameMsgMgr->ShowMessageBox("Popup_Store_Dia");
	}
}

void UPSWidgetSlot_Store::TryPurchaseGold()
{
	if (g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->m_SelectedGoodsPrice = FCString::Atoi(*strGoodsPrice);
		g_PSCharacterMgr->m_SelectedGoodsNumber = GoodsNumber;
	}
	if(g_GameMsgMgrValid)
	{
		g_GameMsgMgr->ShowMessageBox("Popup_Store_Gold");
	}
}

void UPSWidgetSlot_Store::SetSlotInfo()
{
	if(!Background) return;
	if(!NumberText) return;
	if(!PriceText) return;
	if(!Icon_Price) return;
	if(!IconImage) return;
	if(!Text_SubPriecText) return;
	NumberText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), GoodsNumber)));
	PriceText->SetText(FText::FromString(strGoodsPrice));
	
		switch (m_eGoodsType)
		{
		case nGoodsType::Dia:
			{
				Background->SetColorAndOpacity(FLinearColor(0.615923,0.11238,1.0,1.000000));
				IconImage->SetBrushFromSpriteName(strSpriteName);
				Text_SubPriecText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}break;
		case nGoodsType::Gold:
			{
				Icon_Price->SetBrushFromSpriteName("Gem_Blue_Icon_01_Sprite");
				Icon_Price->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Background->SetColorAndOpacity(FLinearColor(0.875000,0.779522,0.147544,1.000000));
				IconImage->SetBrushFromSpriteName(strSpriteName);
			}break;
		case nGoodsType::FreeGold:
			{
				Background->SetColorAndOpacity(FLinearColor(0.875000,0.779522,0.147544,1.000000));
				IconImage->SetBrushFromSpriteName(strSpriteName);
				DisableFreeGold();
			}break;
			default:
				break;
		}
	
}

void UPSWidgetSlot_Store::DisableFreeGold()
{
	if(g_PSCharacterMgr->GetDisableFreeGold())
	{
		if(SlotButton)
		{
			SlotButton->SetIsEnabled(false);
		}

		if(Overlay_Timer)
		{
			Overlay_Timer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		FDateTime DateTime = FDateTime::UtcNow();

		int32 hour =  DateTime.GetHour();
		int32 minute = DateTime.GetMinute();
		int32 seconds = DateTime.GetSecond();

		if(TextBlock_FreeGoldTime)
		{
			FString strText = FString::FromInt(hour) + TEXT(":") +FString::FromInt(minute) + TEXT(":")+FString::FromInt(seconds);
			TextBlock_FreeGoldTime->SetText(FText::FromString(strText));
		}
	}
}
