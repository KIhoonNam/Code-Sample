// Fill out your copyright notice in the Description page of Project Settings.


#include "PSScrollWidget_Store.h"

#include "PSWidgetSlot_Store.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"

void UPSScrollWidget_Store::InitData()
{
	Super::InitData();
}

void UPSScrollWidget_Store::ReleaseData()
{
	Super::ReleaseData();
}

void UPSScrollWidget_Store::LinkEvent()
{
	Super::LinkEvent();
}

void UPSScrollWidget_Store::UnLinkEvent()
{
	Super::UnLinkEvent();
}

void UPSScrollWidget_Store::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);

	if (UPSWidgetSlot_Store* pStoreSlot = Cast<UPSWidgetSlot_Store>(_pChildWidget))
	{
		nGoodsType::en eGoodsType = pStoreSlot->GetGoodsType();

		switch (eGoodsType)
		{
		case nGoodsType::Dia:
			{
				pStoreSlot->TryPurchaseDia();
			}break;
		case nGoodsType::Gold:
			{
				pStoreSlot->TryPurchaseGold();
			}break;
		case nGoodsType::FreeGold:
			{
				GetFreeGold(pStoreSlot->GoodsNumber);
				pStoreSlot->DisableFreeGold();
			}break;
			default:
				break;
		}
	}
}

void UPSScrollWidget_Store::GetFreeGold(int32 numberGold)
{
	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->SetDisableFreeGold(true);
		g_PSCharacterMgr->SetGold(g_PSCharacterMgr->GetGold() + numberGold);
		g_PSCharacterMgr->InitCharacterStat();
		g_DataBaseMgr->OnOverWeaponLevelup(EFirebaseDatabaseError::None);
		g_DataBaseMgr->SetStoreInfo(TEXT("FreeGold"),true);
	}
}
