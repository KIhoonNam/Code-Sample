// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_MyEquip.h"

#include "PSWidgetSlot_MyEquip.h"
#include "Animation/WidgetAnimation.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"

void UPSWidget_MyEquip::InitData()
{
	Super::InitData();

	if(Grid_MyEquip)
	{
		for(UWidget* pWidget :Grid_MyEquip->GetAllChildren())
		{
			if(UPSWidgetSlot_MyEquip* pSlost = Cast<UPSWidgetSlot_MyEquip>(pWidget))
			{
				m_arrMyEquip.Emplace(pSlost);
			}
		}
	}
	if(g_DataBaseMgrValid)
	{
		if(g_DataBaseMgr->GetIsLoad())
		{
			LoadCompleteDatabase();
		}
	}
}

void UPSWidget_MyEquip::LinkEvent()
{
	Super::LinkEvent();
	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this,&UPSWidget_MyEquip::LoadCompleteDatabase);
		g_DataBaseMgr->WeaponLevelup.AddUObject(this,&UPSWidget_MyEquip::OnWeaponLevelup);
	}

}

void UPSWidget_MyEquip::UnLinkEvent()
{
	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
		g_DataBaseMgr->WeaponLevelup.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidget_MyEquip::SetEquipmentType(nEquipmentType::en _eEquipmentType)
{
	m_eEquipmentType = _eEquipmentType;
	switch (_eEquipmentType)
	{
	case nEquipmentType::Equip:
		{
			if(Anim_Equip)
			{
				PlayAnimation(Anim_Equip);
			}
		}break;
	case nEquipmentType::Info:
		{
			if(Anim_Info)
			{
				PlayAnimation(Anim_Info);
			}
		}break;
		default:
			break;
	}

}

int32 UPSWidget_MyEquip::GetEmptyEquip()
{
	int32 Index = -1;
	
	for(int number = 0; number < m_arrMyEquip.Num();number++)
	{
		if(!m_arrMyEquip[number]->GetWeaponStat())
		{
			Index = number;
			break;
		}
	}
	return Index;
}

void UPSWidget_MyEquip::SetEmptyEquip(UPSWidgetSlot_MyEquip* pSlot)
{
	if(pSlot)
	{
		if(m_arrMyEquip.Contains(pSlot))
		{
			int32 index = m_arrMyEquip.Find(pSlot);
			g_PSCharacterMgr->ChangeEquipWeapon(index,pSlot->GetWeaponStat(),false);
			pSlot->SetEmptyStat();
		}
	}
}

bool UPSWidget_MyEquip::TryEquip(FWeaponStat* pWeaponStat)
{
	bool isEquip = false;

	if(!pWeaponStat) return isEquip;
	int32 emptyIndex = GetEmptyEquip();

	if(emptyIndex == -1) return isEquip;
	
		
	if(m_arrMyEquip.IsValidIndex(emptyIndex))
	{
		m_arrMyEquip[emptyIndex]->SetWeaponStat(pWeaponStat);
		g_PSCharacterMgr->ChangeEquipWeapon(emptyIndex,pWeaponStat,true);
		isEquip = true;
	}

	return isEquip;
}

void UPSWidget_MyEquip::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);

	if(UPSWidgetSlot_MyEquip* pSlot = Cast<UPSWidgetSlot_MyEquip>(_pChildWidget))
	{
		switch (m_eEquipmentType)
		{
		case nEquipmentType::Equip:
			{
				if(m_pParentWidget.IsValid())
				{
					m_pParentWidget->OnChildItemClicked(_pChildWidget);
				}
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

void UPSWidget_MyEquip::LoadCompleteDatabase()
{
	if(g_PSCharacterMgrValid)
	{
		if(Grid_MyEquip)
		{
			int32 WeaponNum = g_PSCharacterMgr->m_mapEquipWeapon.Num();
			TArray<int32> arrEquipWeaponKeys;
			g_PSCharacterMgr->m_mapEquipWeapon.GetKeys(arrEquipWeaponKeys);
			
			for(int32 Number : arrEquipWeaponKeys)
			{
				if(m_arrMyEquip.IsValidIndex(Number))
				{
					if(FWeaponStat* pWeaponStat =g_PSCharacterMgr->m_mapEquipWeapon[Number] )
					{
						if(UPSWidgetSlot_MyEquip* pSlot = m_arrMyEquip[Number].Get())
						{
							pSlot->SetWeaponStat(pWeaponStat);
							pSlot->SetClickEvent(this);
						}
					}
				}
			}
		}
	}
}

void UPSWidget_MyEquip::OnWeaponLevelup(const FString& String, int level, int piece)
{
	if(Grid_MyEquip)
	{
		for(UWidget* pWidget : Grid_MyEquip->GetAllChildren())
		{
			if(UPSWidgetSlot_MyEquip* pSlot = Cast<UPSWidgetSlot_MyEquip>(pWidget))
			{
				FWeaponStat* pWeapon = pSlot->GetWeaponStat();
				if(pWeapon)
				{
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
		}

	}
}
