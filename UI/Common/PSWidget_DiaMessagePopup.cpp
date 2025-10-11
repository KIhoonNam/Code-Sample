// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_DiaMessagePopup.h"

#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidget_DiaMessagePopup::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (PopupAnimation)
	{
		PlayAnimation(PopupAnimation);
	}

	if (g_PSCharacterMgrValid)
	{
		if (StoreDiaText)
		{
			int32 price = g_PSCharacterMgr->m_SelectedGoodsPrice;
			int32 number = g_PSCharacterMgr->m_SelectedGoodsNumber;
			StoreDiaText->SetText(FText::FromString(FString::Printf(TEXT("%d원을 결제하여\n다이아 %d개를\n구매하시겠습니까?"), price, number)));
		}
	}
}

void UPSWidget_DiaMessagePopup::OnClickedOKEvent()
{
	Super::OnClickedOKEvent();
	
	if (g_PSPurchaseMgrValid)
	{
		FString strProductName = g_CSDefineRecordMgr->GetValue_String("product_dia");
		strProductName += FString::Printf(TEXT("%d"), g_PSCharacterMgr->m_SelectedGoodsPrice);
		g_PSPurchaseMgr->Purchase_Dia(0, strProductName);
	}
}
