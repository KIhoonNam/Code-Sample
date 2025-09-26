// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_GoldMessagePopup.h"

#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidget_GoldMessagePopup::InitData()
{
	Super::InitData();

	if (g_PSCharacterMgrValid)
	{
		if (StoreGoldText)
		{
			int32 price = g_PSCharacterMgr->m_SelectedGoodsPrice;
			int32 number = g_PSCharacterMgr->m_SelectedGoodsNumber;
			StoreGoldText->SetText(FText::FromString(FString::Printf(TEXT("다이아 %d개를 사용하여\n골드 %d개를\n구매하시겠습니까?"), price, number)));
		}
	}
}

void UPSWidget_GoldMessagePopup::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (PopupAnimation)
	{
		PlayAnimation(PopupAnimation);
	}
}

void UPSWidget_GoldMessagePopup::OnClickedOKEvent()
{
	if (g_PSCharacterMgrValid)
	{
		if (g_PSCharacterMgr->m_SelectedGoodsPrice > g_PSCharacterMgr->GetDia())
		{
			if (g_GameMsgMgrValid)
			{
				g_GameMsgMgr->ShowMessageBox("Popup_Not_Enough_Dia");
			}
		}
		else
		{
			if (g_PSCharacterMgrValid)
			{
				g_PSCharacterMgr->SetGold(g_PSCharacterMgr->GetGold() + g_PSCharacterMgr->m_SelectedGoodsNumber);
				g_PSCharacterMgr->SetDia(g_PSCharacterMgr->GetDia() - g_PSCharacterMgr->m_SelectedGoodsPrice);
			}
			Super::OnClickedOKEvent();
		}
	}
}
