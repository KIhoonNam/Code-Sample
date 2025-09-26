// Fill out your copyright notice in the Description page of Project Settings.


#include "PSScrollWidget_CharacterStore.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"
#include "PSWidgetSlot_CharacterStatStore.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"

void UPSScrollWidget_CharacterStore::InitData()
{
	Super::InitData();

	if(ScrollBox_Base)
	{
	
			for(UCSWidgetSlot* pWidget : ScrollBox_Base->GetWidgetChildren())
			{

					pWidget->SetClickEvent(this);
				
			}
		
	}
	if(g_DataBaseMgrValid)
	{
		if(g_DataBaseMgr->GetIsLoad())
		{
			RefreshSlot();
		}
	}


}

void UPSScrollWidget_CharacterStore::LinkEvent()
{
	Super::LinkEvent();

	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this,&UPSScrollWidget_CharacterStore::RefreshSlot);
	}
}

void UPSScrollWidget_CharacterStore::UnLinkEvent()
{
	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
	}
	Super::UnLinkEvent();
}

TArray<UPSWidgetSlot_CharacterStatStore*> UPSScrollWidget_CharacterStore::GetSlots()
{
	TArray<UPSWidgetSlot_CharacterStatStore*> arrSlots;

	if(ScrollBox_Base)
	{
		for(UWidget* pWidget : ScrollBox_Base->GetAllChildren())
		{
			if(UPSWidgetSlot_CharacterStatStore* pSlot = Cast<UPSWidgetSlot_CharacterStatStore>(pWidget))
			{
				arrSlots.Emplace(pSlot);
			}
		}
	}

	return arrSlots;
}

void UPSScrollWidget_CharacterStore::RefreshSlot()
{
	TArray<UPSWidgetSlot_CharacterStatStore*> arrSlots = GetSlots();

	for(UPSWidgetSlot_CharacterStatStore* pSlot : arrSlots)
	{
		pSlot->RefreshStat();
	}
}
