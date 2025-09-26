// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_Equipment.h"

#include "PSWidget_SignalMark.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponPieceRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSWeaponDetailRecord.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSWeaponPieceRecord.h"

#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSCanvasPanel.h"
#include "Components/ProgressBar.h"
void UPSWidgetSlot_Equipment::SetWeaponStat(FWeaponStat* WeaponStat)
{
	if(WeaponStat)
	{
		m_pWeaponStat = WeaponStat;
	}

	if(m_pWeaponStat)
	{
		
		if(Image_Weapon)
		{
			Image_Weapon->SetBrushFromSpriteName(m_pWeaponStat->m_strSpriteName);
		}

		if(Image_Rate)
		{
			FString strRateSPR = m_pWeaponStat->m_strRateSpriteName + TEXT("_Long");
			Image_Rate->SetBrushFromSpriteName(strRateSPR);
		}
		
		if(Text_Name)
		{
			Text_Name->SetText(FText::FromString(m_pWeaponStat->m_strName));
		}
		
		if(Text_Level)
		{
			FString strLevel = TEXT("Lv.") + FString::FromInt(WeaponStat->Level);
			Text_Level->SetText(FText::FromString(strLevel));
		}

		if(Text_Piece)
		{
			if(PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(WeaponStat->Level)))
			{
				if(pRecord->m_mapPiece.Contains(m_pWeaponStat->m_eRateType))
				{
					int32 maxPiece = pRecord->m_mapPiece[m_pWeaponStat->m_eRateType];
					FString strPiece = FString::FromInt(WeaponStat->Piece) + TEXT("/") + FString::FromInt(maxPiece);
					Text_Piece->SetText(FText::FromString(strPiece));
				}
			}
		}

		if(Canvas_Lock)
		{
			if(m_pWeaponStat->m_IsLock)
			{
				Canvas_Lock->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		UpdateMark(m_pWeaponStat);
	}	
}

void UPSWidgetSlot_Equipment::UpdateMark(FWeaponStat* WeaponStat)
{
	int32 m_Level = WeaponStat->Level;

	bool bCanLevelUp = false;

	if (m_Level > 0)
	{
		if (PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(m_Level)))
		{
			m_eRateType = WeaponStat->m_eRateType;
			if (pRecord->m_mapPiece.Contains(m_eRateType))
			{
				int32 m_MaxPiece = pRecord->m_mapPiece[m_eRateType];
				int32 m_MaxGold = pRecord->m_mapGold[m_eRateType];
                
				bCanLevelUp = (WeaponStat->Piece >= m_MaxPiece) && (g_PSCharacterMgr->GetGold() >= m_MaxGold) && WeaponStat->m_IsLock;
			}
		}
	}
	if (W_SignalMark_BP)
	{
		W_SignalMark_BP->SetVisibility(bCanLevelUp ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		
		if (bCanLevelUp)
		{
			W_SignalMark_BP->SyncAnimation();
		}
	}
	UpdateProgressBar();
}

void UPSWidgetSlot_Equipment::UpdateProgressBar()
{
	if (ProgressBar_Piece && m_pWeaponStat)
	{
		if (PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(m_pWeaponStat->Level)))
		{
			if (pRecord->m_mapPiece.Contains(m_pWeaponStat->m_eRateType))
			{
				int32 maxPiece = pRecord->m_mapPiece[m_pWeaponStat->m_eRateType];
				float progress = (maxPiece > 0) ? static_cast<float>(m_pWeaponStat->Piece) / static_cast<float>(maxPiece) : 0.0f;

				ProgressBar_Piece->SetPercent(FMath::Clamp(progress, 0.0f, 1.0f));

				FLinearColor fillColor = (progress >= 1.0f) 
										 ? FLinearColor(0.0f, 0.796f, 0.184f, 1.0f)
										 : FLinearColor(1.0f, 0.718f, 0.0f, 1.0f);
				ProgressBar_Piece->SetFillColorAndOpacity(fillColor);
			}
		}
	}
}



