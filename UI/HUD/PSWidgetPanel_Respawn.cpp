// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetPanel_Respawn.h"

#include "ProtoSurvivor/Manager/PSAdMobMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"

void UPSWidgetPanel_Respawn::InitData()
{
	Super::InitData();

	if (g_PSCharacterMgrValid)
	{
		if (ButtonSwitcher)
		{
			int32 switcherIndex = g_PSCharacterMgr->GetPurchasedAdRemove() == true ? 1 : 0;
			ButtonSwitcher->SetActiveWidgetIndex(switcherIndex);
		}
	}
}

void UPSWidgetPanel_Respawn::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidgetPanel_Respawn::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Cancel, &UPSWidgetPanel_Respawn::OnClicked_Cancel);
	BIND_BUTTON_EVENT(Button_Ad, &UPSWidgetPanel_Respawn::OnClicked_Respawn);
	BIND_BUTTON_EVENT(Button_Respawn, &UPSWidgetPanel_Respawn::OnClicked_Respawn);
	
	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.AddUObject(this, &UPSWidgetPanel_Respawn::ApplyAdRemove);
	}
}

void UPSWidgetPanel_Respawn::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Cancel);
	UNBIND_BUTTON_EVENT_ALL(Button_Ad);
	UNBIND_BUTTON_EVENT_ALL(Button_Respawn);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidgetPanel_Respawn::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->AddPauseReason(GetWorld(),nPauseType::ReSpawn);
	}
}

void UPSWidgetPanel_Respawn::OnClicked_Cancel()
{
	if(g_UIMgrValid)
	{
		g_UIMgr->HideUIPanel(nUIPanelType::Respawn);
		g_UIMgr->ShowUIPanel(nUIPanelType::StageClear);
		if(g_PSPauseMgrValid)
		{
			if (g_PSPauseMgrValid)
			{
				g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::ReSpawn);
			}
		}
	}
}

void UPSWidgetPanel_Respawn::OnClicked_Respawn()
{
	if (g_PSAdMobMgrValid)
	{
		g_PSAdMobMgr->ShowRewardAd_Respawn(0, "Respawn");
	}
}

void UPSWidgetPanel_Respawn::ApplyAdRemove()
{
	if (ButtonSwitcher)
	{
		ButtonSwitcher->SetActiveWidgetIndex(1);
	}
}


