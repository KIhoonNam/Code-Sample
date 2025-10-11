// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_ResultMessagePopup.h"

#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidget_ResultMessagePopup::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if(Text_Desc)
	{
		FString strDesc = g_CSDefineRecordMgr->GetValue_String(m_PopupValue.m_strMsgDesc);
		Text_Desc->SetText(FText::FromString(strDesc));
	}
	
	PlayAnimation(ResultPopupAnimation);
}

void UPSWidget_ResultMessagePopup::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	RemoveFromParent();
}
