// Fill out your copyright notice in the Description page of Project Settings.

#include "PSHUD.h"

#include "UILibrary/Manager/CSUIMgr.h"

void APSHUD::CreateWidgetPanels()
{
	if(g_UIMgrValid)
	{
		g_UIMgr->ClearPanels();

		TArray<SPanelTypeInfo> _Infos;
		
		_Infos.Add(SPanelTypeInfo(nUIPanelType::Play, FName("Play")));
		_Infos.Add(SPanelTypeInfo(nUIPanelType::StageClear, FName("StageClear")));
		_Infos.Add(SPanelTypeInfo(nUIPanelType::Respawn, FName("Respawn")));
		_Infos.Add(SPanelTypeInfo(nUIPanelType::WeaponInfo, FName("WeaponInfo")));
		g_UIMgr->CreatePanels(_Infos, GetWorld());
	}
}
