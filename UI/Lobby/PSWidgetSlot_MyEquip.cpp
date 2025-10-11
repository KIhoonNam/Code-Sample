// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_MyEquip.h"

#include "PSWidget_SignalMark.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponPieceRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSWeaponPieceRecord.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "ProtoSurvivor/UI/Common/PSWidget_WeaponInfo.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidgetSlot_MyEquip::SetWeaponStat(FWeaponStat* WeaponStat)
{
	if(WeaponStat)
	{
		m_pWeaponStat = WeaponStat;

		if(Image_Weapon)
		{
			Image_Weapon->SetBrushFromSpriteName(m_pWeaponStat->m_strSpriteName);
		}

		if(Image_Rate)
		{
			Image_Rate->SetBrushFromSpriteName(m_pWeaponStat->m_strRateSpriteName);
		}

		if(Text_Level)
		{
			FString strLevel = FString::FromInt(m_pWeaponStat->Level);
			Text_Level->SetText(FText::FromString(strLevel));
		}
		
		if(WidgetSwitcher_Equip)
		{
			WidgetSwitcher_Equip->SetActiveWidgetIndex(1);
		}
		
		UpdateMark(m_pWeaponStat);
	}
}

void UPSWidgetSlot_MyEquip::UpdateMark(FWeaponStat* WeaponStat)
{
	int32 m_Level = WeaponStat->Level;
	if(PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(m_Level)))
	{
		m_eRateType = WeaponStat->m_eRateType;
		if(pRecord->m_mapPiece.Contains(m_eRateType))
		{
			int32 m_MaxPiece = pRecord->m_mapPiece[m_eRateType];
			int32 m_MaxGold= pRecord->m_mapGold[m_eRateType];
			
			bool bCanLevelUp = (WeaponStat->Piece >= m_MaxPiece) && (g_PSCharacterMgr->GetGold() >= m_MaxGold);

			if (W_SignalMark_BP)
			{
				W_SignalMark_BP->SetVisibility(bCanLevelUp ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
				W_SignalMark_BP->SyncAnimation();
			}
		}
	}
}

void UPSWidgetSlot_MyEquip::SetEmptyStat()
{
	m_pWeaponStat = nullptr;

	if(WidgetSwitcher_Equip)
	{
		WidgetSwitcher_Equip->SetActiveWidgetIndex(0);
	}
}
