// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_CombatSpeed.h"

#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Manager/PSAdMobMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/UI/HUD/PSWidgetPanel_Play.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "ProtoSurvivor/UI/Play/PSWidget_AdRemove.h"

void UPSWidget_CombatSpeed::InitData()
{
	Super::InitData();
}

void UPSWidget_CombatSpeed::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_CombatSpeed::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Ad, &UPSWidget_CombatSpeed::OnClicked_Ad);
	BIND_BUTTON_EVENT(Button_Cancel, &UPSWidget_CombatSpeed::OnClicked_Cancel);
}

void UPSWidget_CombatSpeed::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Ad);
	UNBIND_BUTTON_EVENT_ALL(Button_Cancel);
	
	Super::UnLinkEvent();
}

void UPSWidget_CombatSpeed::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (AdRemoveWidget)
	{
		AdRemoveWidget->Show();
	}
	
	if (Animation_Open)
	{
		PlayAnimation(Animation_Open);
	}
}

void UPSWidget_CombatSpeed::OnClicked_Cancel()
{
	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::Speed);
	}
	Hide();
}

void UPSWidget_CombatSpeed::OnClicked_Ad()
{
	if (g_PSAdMobMgrValid)
	{
		g_PSAdMobMgr->ShowRewardAd_CombatSpeed(0, "CombatSpeed", this);
	}
}

void UPSWidget_CombatSpeed::SetCanChangeCombatSpeed()
{
	if (UPSWidgetPanel_Play* pPlayWidgetPanel = Cast<UPSWidgetPanel_Play>(GetParentWidget()))
	{
		pPlayWidgetPanel->SetCanChangeCombatSpeed(true);
		Hide();
		if (g_PSPauseMgrValid)
		{
			g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::Speed);
		}
	}
}
