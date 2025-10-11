// Fill out your copyright notice in the Description page of Project Settings.


#include "PSScrollWidget_Equipment.h"

#include "PSWidgetSlot_Equipment.h"
#include "Components/UniformGridSlot.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
void UPSScrollWidget_Equipment::InitData()
{
	Super::InitData();

	if(g_DataBaseMgrValid)
	{
		if(g_DataBaseMgr->GetIsLoad())
		{
			LoadCompleteDatabase();
		}
	}
}

void UPSScrollWidget_Equipment::LinkEvent()
{
	Super::LinkEvent();


	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this,&UPSScrollWidget_Equipment::LoadCompleteDatabase);
		g_DataBaseMgr->WeaponLevelup.AddUObject(this,&UPSScrollWidget_Equipment::WeaponLevelUp);
	}


	
}

void UPSScrollWidget_Equipment::UnLinkEvent()
{
	
	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
		g_DataBaseMgr->WeaponLevelup.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSScrollWidget_Equipment::ReleaseData()
{
	Super::ReleaseData();
}

void UPSScrollWidget_Equipment::SetEquipmentType(nEquipmentType::en _eEquipmentType)
{
	m_eEquipmentType = _eEquipmentType;

	switch (m_eEquipmentType)
	{
	case nEquipmentType::Equip:
		{
			ShowAndHideWeapon(ESlateVisibility::Collapsed);
		}break;
	case nEquipmentType::Info:
		{
			ShowAndHideWeapon(ESlateVisibility::SelfHitTestInvisible);
		}break;
		default:
			break;
	}
}

void UPSScrollWidget_Equipment::LoadCompleteDatabase()
{
	if(g_PSCharacterMgrValid)
	{
		if(Grid_Equipment)
		{
			Grid_Equipment->ClearChildren();

			int32 WeaponNum = g_PSCharacterMgr->m_MapWeapon.Num();
			TArray<FString> arrWeaponKeys;
			g_PSCharacterMgr->m_MapWeapon.GetKeys(arrWeaponKeys);
			
			for(int Num = 0 ; Num < WeaponNum; Num++)
			{
				if(arrWeaponKeys.IsValidIndex(Num))
				{
					if(FWeaponStat* pWeaponStat =g_PSCharacterMgr->m_MapWeapon[arrWeaponKeys[Num]] )
					{
						if(pWeaponStat->m_strTid.Contains("cost")) continue;
						if(UPSWidgetSlot_Equipment* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_Equipment>("S_Equipment_Bp",GetWorld()))
						{

							Grid_Equipment->AddChild(pSlot);
				
							pSlot->SetWeaponStat(pWeaponStat);
							pSlot->SetClickEvent(this);

							if(pWeaponStat->m_IsEquip)
							{
								pSlot->SetVisibility(ESlateVisibility::Collapsed);
							}
						}
					}
				}
			}
			OrderListItem();
			OrderListGrid();
		}
	}
}

void UPSScrollWidget_Equipment::WeaponLevelUp(const FString& String, int level, int piece)
{
	if(Grid_Equipment)
	{
		for(UWidget* pWidget : Grid_Equipment->GetAllChildren())
		{
			if(UPSWidgetSlot_Equipment* pSlot = Cast<UPSWidgetSlot_Equipment>(pWidget))
			{
				FWeaponStat* pWeapon = pSlot->GetWeaponStat();

				if(String  == pWeapon->m_strTid)
				{
					pWeapon->Level = level;
					pWeapon->Piece = piece;

					pSlot->SetWeaponStat(pWeapon);
				}
				else
				{
					pSlot->UpdateMark(pWeapon);
				}
			}
		}

		OrderListItem();
		OrderListGrid();
	}
	
}

void UPSScrollWidget_Equipment::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);

	if(UPSWidgetSlot_Equipment* pSlot = Cast<UPSWidgetSlot_Equipment>(_pChildWidget))
	{
		switch (m_eEquipmentType)
		{
		case nEquipmentType::Equip:
			{
			
			}break;
		case nEquipmentType::Info:
			{
				if(pSlot->GetWeaponStat())
				{
					g_PSCharacterMgr->m_pWeaponStat = pSlot->GetWeaponStat();
					g_UIMgr->ShowUIPanel(nUIPanelType::WeaponInfo);
				}
			}break;
		default:
			break;
		}
	}
}

void UPSScrollWidget_Equipment::ShowAndHideWeapon(ESlateVisibility visible)
{
	if(Grid_Equipment)
	{
		for(UWidget* pSLot :Grid_Equipment->GetAllChildren())
		{
			if(UPSWidgetSlot_Equipment* pEquipmentSlot = Cast<UPSWidgetSlot_Equipment>(pSLot))
			{
				if(FWeaponStat* pWeapon = pEquipmentSlot->GetWeaponStat())
				{
					if(!pWeapon->m_IsLock)
					{
						pEquipmentSlot->SetVisibility(visible);
					}
				}
			}
		}
	}
}

void UPSScrollWidget_Equipment::Unfitted(FWeaponStat* pWeaponStat)
{
	if(pWeaponStat)
	{
		pWeaponStat->m_IsEquip = false;

		if(Grid_Equipment)
		{
			for(UWidget* pSLot :Grid_Equipment->GetAllChildren())
			{
				if(UPSWidgetSlot_Equipment* pEquipmentSlot = Cast<UPSWidgetSlot_Equipment>(pSLot))
				{
					if(pEquipmentSlot->GetWeaponStat())
					{
						if(pEquipmentSlot->GetWeaponStat()->m_strName == pWeaponStat->m_strName)
						{
							pEquipmentSlot->SetWeaponStat(pWeaponStat);
							pEquipmentSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
							break;
						}
					}

				
				}
			}
		}
	}
}

void UPSScrollWidget_Equipment::OrderListItem()
{

		if (!m_WeaponDetailData.empty())
		{
			m_WeaponDetailData.clear();
		}
		
		TArray<FString> arrWeaponDetailKeys;
		g_PSCharacterMgr->m_MapWeapon.GetKeys(arrWeaponDetailKeys);
		for (int32 WeaponIndex = 0; WeaponIndex < arrWeaponDetailKeys.Num(); WeaponIndex++)
		{
			if (FWeaponStat* WeaponStatInfo = g_PSCharacterMgr->m_MapWeapon[arrWeaponDetailKeys[WeaponIndex]])
			{
				FName resultKey = FName(FString::Printf(TEXT("%d_%d_%d_%d_%d"),
					static_cast<int32>(!WeaponStatInfo->m_IsEquip),
					WeaponStatInfo->m_IsLock,
					static_cast<int32>(WeaponStatInfo->m_eRateType),
					static_cast<int32>(WeaponStatInfo->Level),
					WeaponIndex
				));
				
				m_WeaponDetailData.insert(std::make_pair(resultKey, WeaponIndex));
			}
		}
	
}

void UPSScrollWidget_Equipment::OrderListGrid()
{

	if(Grid_Equipment)
	{
		int32 columnMax =g_CSDefineRecordMgr->GetValue_Number("GV_EQUIPMENT_Column");

			int32 slotIndex = 0;
			for (auto& customizeItem: m_WeaponDetailData)
			{
				if (UPSWidgetSlot_Equipment* pSlot = Cast<UPSWidgetSlot_Equipment>(Grid_Equipment->GetChildAt(customizeItem.second)))
				{
						if(UUniformGridSlot* pGridSlot = Cast<UUniformGridSlot>(pSlot->Slot))
						{
							pGridSlot->SetColumn(slotIndex % columnMax);
							pGridSlot->SetRow(slotIndex / columnMax);

							pGridSlot->SetHorizontalAlignment(HAlign_Center);
							pGridSlot->SetVerticalAlignment(VAlign_Top);
							
							slotIndex++;
						}
				}
			}
		}
	
}
