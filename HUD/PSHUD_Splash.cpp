// Fill out your copyright notice in the Description page of Project Settings.


#include "PSHUD_Splash.h"

#include "UILibrary/Manager/CSUIMgr.h"

void APSHUD_Splash::CreateWidgetPanels()
{
	if(g_UIMgrValid)
	{
		g_UIMgr->ClearPanels();

		TArray<SPanelTypeInfo> _Infos;
		_Infos.Add(SPanelTypeInfo(nUIPanelType::Splash, FName("Splash")));
		g_UIMgr->CreatePanels(_Infos, GetWorld());



	}
}
