// Fill out your copyright notice in the Description page of Project Settings.

#include "PSWidgetPanel_Lobby.h"

#include "PSWidgetSlot_LobbyMenu.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/UI/Play/PSWidget_AdRemove.h"
#include "ProtoSurvivor/UI/Play/PSWidget_Sound.h"
#include "ProtoSurvivor/UI/Play/PSWidget_WaveBackPack.h"
#include "ProtoSurvivor/UI/Stage/PSWidget_Stage.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"

void UPSWidgetPanel_Lobby::InitData()
{
	Super::InitData();


	if (g_SoundMgrValid)
	{
		g_SoundMgr->PlaySound2D("Sound_LobbyBGM_Cue");
	}
}

void UPSWidgetPanel_Lobby::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidgetPanel_Lobby::LinkEvent()
{
	Super::LinkEvent();

	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->MoveGoodsStore.BindUObject(this,&UPSWidgetPanel_Lobby::SelectScrollBoxLobbyChild);
	}
}

void UPSWidgetPanel_Lobby::UnLinkEvent()
{
	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->MoveGoodsStore.Unbind();
	}
	Super::UnLinkEvent();
}

void UPSWidgetPanel_Lobby::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);

	if(UPSWidgetSlot_LobbyMenu* pSlot = Cast<UPSWidgetSlot_LobbyMenu>(_pChildWidget))
	{
		int32 numberMenu = pSlot->MenuNumber;

		VisibleMenu(numberMenu);
	}
}

void UPSWidgetPanel_Lobby::VisibleMenu(int32 _numberMenu)
{
	if(WidgetSwitcher_Lobby)
	{
		WidgetSwitcher_Lobby->SetActiveWidgetIndex(_numberMenu);

		if (UPSWidget_Stage* pStageWidget = Cast<UPSWidget_Stage>(WidgetSwitcher_Lobby->GetChildAt(1)))
		{
			pStageWidget->AdRemoveWidget->Hide();
			pStageWidget->SoundWidget->Hide();
		}
	}
}

void UPSWidgetPanel_Lobby::SelectScrollBoxLobbyChild(int32 _numberMenu)
{
	if(Widget_ScrollboxLobby)
	{
		Widget_ScrollboxLobby->AddSelectedChild(_numberMenu);
		VisibleMenu(_numberMenu);
	}
}
