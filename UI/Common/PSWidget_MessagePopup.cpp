// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_MessagePopup.h"

#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidget_MessagePopup::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);


	if(Text_Title)
	{
		FString strTitle = g_CSDefineRecordMgr->GetValue_String(m_PopupValue.m_strTitle);
		Text_Title->SetText(FText::FromString(strTitle));
	}

	if(Text_Desc)
	{
		FString strDesc = g_CSDefineRecordMgr->GetValue_String(m_PopupValue.m_strMsgDesc);
		Text_Desc->SetText(FText::FromString(strDesc));
	}

}
