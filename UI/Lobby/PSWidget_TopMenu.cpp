// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_TopMenu.h"

#include "PSWidgetPanel_Lobby.h"
#include "PSWidgetSlot_TopMenu.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
void UPSWidget_TopMenu::InitData()
{
	Super::InitData();

	
}

void UPSWidget_TopMenu::LinkEvent()
{
	Super::LinkEvent();
	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->ChangedGold.AddUObject(this,&UPSWidget_TopMenu::ChangedGoods);
	}
}

void UPSWidget_TopMenu::UnLinkEvent()
{
	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->ChangedGold.RemoveAll(this);
	}
	Super::UnLinkEvent();
}

void UPSWidget_TopMenu::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);


	if(UPSWidgetSlot_TopMenu* pTopMenuSlot = Cast<UPSWidgetSlot_TopMenu>(_pChildWidget))
	{
		nGoodsType::en eGoodsType = pTopMenuSlot->GetGoodsType();
		switch (eGoodsType)
		{
		case nGoodsType::Gold:
		case nGoodsType::Dia:
			{
				if(g_PSCharacterMgrValid)
				{
					g_PSCharacterMgr->OnMoveGoodsStore(3);
				}
			}
			break;

		case nGoodsType::Energy:
			{
				
			}break;
			default:
				break;
		}
	}
}

void UPSWidget_TopMenu::ChangedGoods(int I, nGoodsType::en En)
{
	if(ScrollBox_Goods)
	{
		for(UCSWidgetSlot* pSlot : ScrollBox_Goods->GetWidgetChildren())
		{
			if(UPSWidgetSlot_TopMenu* pTopMenuSlot = Cast<UPSWidgetSlot_TopMenu>(pSlot))
			{
				pTopMenuSlot->ChangeGoods();
			}
		}
	}
}
