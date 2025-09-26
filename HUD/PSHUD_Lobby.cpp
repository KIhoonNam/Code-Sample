// Fill out your copyright notice in the Description page of Project Settings.


#include "PSHUD_Lobby.h"

#include "UILibrary/Define/CSDefine_UICommon.h"
#include "UILibrary/Manager/CSUIMgr.h"


void APSHUD_Lobby::CreateWidgetPanels()
{
	if(g_UIMgrValid)
	{
		g_UIMgr->ClearPanels();

		TArray<SPanelTypeInfo> _Infos;
		_Infos.Add(SPanelTypeInfo(nUIPanelType::Lobby, FName("Lobby")));
		_Infos.Add(SPanelTypeInfo(nUIPanelType::WeaponInfo, FName("WeaponInfo")));
		g_UIMgr->CreatePanels(_Infos, GetWorld());



	}
}
