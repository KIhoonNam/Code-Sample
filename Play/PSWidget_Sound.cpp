// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_Sound.h"

#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidget_Sound::InitData()
{
	Super::InitData();

	m_strMute = g_CSDefineRecordMgr->GetValue_String("ui_sound_mute");
	m_strUnmute = g_CSDefineRecordMgr->GetValue_String("ui_sound_unmute");
	
	setText_Effect();
	setText_BGM();
}

void UPSWidget_Sound::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_Sound::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Effect, &UPSWidget_Sound::OnClicked_Effect);
	BIND_BUTTON_EVENT(Button_BGM, &UPSWidget_Sound::OnClicked_BGM);
	BIND_BUTTON_EVENT(Button_Cancel, &UPSWidget_Sound::OnClicked_Cancel);
}

void UPSWidget_Sound::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Effect);
	UNBIND_BUTTON_EVENT_ALL(Button_BGM);
	UNBIND_BUTTON_EVENT_ALL(Button_Cancel);
	
	Super::UnLinkEvent();
}

void UPSWidget_Sound::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (Animation_Open)
	{
		PlayAnimation(Animation_Open);
	}
}

void UPSWidget_Sound::OnClicked_Effect()
{
	if (g_SoundMgrValid)
	{
		g_SoundMgr->SetEffectMute(!g_SoundMgr->GetIsEffectMute());
		setText_Effect();
	}
}

void UPSWidget_Sound::OnClicked_BGM()
{
	if (g_SoundMgrValid)
	{
		g_SoundMgr->SetBGMMute(!g_SoundMgr->GetIsBGMMute());
		setText_BGM();
	}
}

void UPSWidget_Sound::OnClicked_Cancel()
{
	Hide();
}

void UPSWidget_Sound::setText_Effect()
{
	if (Text_Effect)
	{
		Text_Effect->SetText(FText::FromString(g_SoundMgr->GetIsEffectMute() ? m_strUnmute : m_strMute));
	}
}

void UPSWidget_Sound::setText_BGM()
{
	if (Text_BGM)
	{
		Text_BGM->SetText(FText::FromString(g_SoundMgr->GetIsBGMMute() ? m_strUnmute : m_strMute));
	}
}
