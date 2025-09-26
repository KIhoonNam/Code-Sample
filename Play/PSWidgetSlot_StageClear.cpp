// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_StageClear.h"

#include "ProtoSurvivor/DataTable/DataManager/PSWeaponDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSWeaponDetailRecord.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"

void UPSWidgetSlot_StageClear::InitData()
{
	Super::InitData();

	if(CreateSlotAnim)
	{
		PlayAnimation(CreateSlotAnim);
	}
}

void UPSWidgetSlot_StageClear::SetGold(int32 amount)
{
	if(Image_Rate)
	{
		Image_Rate->SetBrushFromSpriteName("SPR_Frame_Green");
	}

	if(Image_Weapon)
	{
		Image_Weapon->SetBrushFromSpriteName("Icon_Reward_Pass_gold_Sprite");
	}


	if(Text_Amount)
	{
		m_Value = amount;
		FString strAmount = FString::FromInt(amount);
		Text_Amount->SetText(FText::FromString(strAmount));
	}
}

void UPSWidgetSlot_StageClear::SetPieceWeapon(FString strTid,int32 amount)
{
	m_strTid = strTid;
	if(PSWeaponDetailRecord* pRecord = g_WeaponDetailRecordMgr->FindRecord(strTid))
	{
		if(pRecord->m_WeaponStat)
		{
			if(Image_Rate)
			{
				Image_Rate->SetBrushFromSpriteName(pRecord->m_WeaponStat->m_strRateSpriteName);	
			}

			if(Image_Weapon)
			{
				Image_Weapon->SetBrushFromSpriteName(pRecord->m_WeaponStat->m_strSpriteName);
			}


			if(Text_Amount)
			{
				m_Value = amount;
				FString strAmount = FString::FromInt(amount);
				Text_Amount->SetText(FText::FromString(strAmount));
			}
		}
	}
}

void UPSWidgetSlot_StageClear::GetClearBonus(int32 multiple)
{
	if(g_PSCharacterMgrValid)
	{
		switch (m_eClearType)
		{
		case nClearType::Gold:
			{
				int32 currentGold = g_PSCharacterMgr->GetGold();
				int32 value = m_Value * multiple;
				g_PSCharacterMgr->SetGold(currentGold+value);
			}break;
		case nClearType::Piece:
			{
				g_PSCharacterMgr->SetWeaponPiece(m_strTid,m_Value*multiple);
			}break;
		case nClearType::Unlock:
			{
				g_PSCharacterMgr->UnlockWeapons(m_strTid);
			}break;
			default:
				break;
		}
	}
}


