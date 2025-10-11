// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_CharacterStatStore.h"
#include "Animation/WidgetAnimation.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "ProtoSurvivor/DataTable/DataManager/PSCharacterStatDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSCharacterStatDetailRecord.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"


void UPSWidgetSlot_CharacterStatStore::InitData()
{
	Super::InitData();


	RefreshText();
	
}

void UPSWidgetSlot_CharacterStatStore::LinkEvent()
{
	Super::LinkEvent();

	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->RefreshStat.AddUObject(this,&UPSWidgetSlot_CharacterStatStore::RefreshStat);
	}
}

void UPSWidgetSlot_CharacterStatStore::UnLinkEvent()
{
	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->RefreshStat.RemoveAll(this);
	}
	Super::UnLinkEvent();
}

void UPSWidgetSlot_CharacterStatStore::PlayClickAnimation()
{
	if(Anim_Click)
	{
		PlayAnimation(Anim_Click);
	}
}

void UPSWidgetSlot_CharacterStatStore::RefreshText()
{
	if(Text_Level)
	{
		FString strLevel = TEXT("Lv ")+ FString::FromInt(m_LevelNumber);
		Text_Level->SetText(FText::FromString(strLevel));
	}

	if(Text_Gold)
	{
		FString strGold = FString::FromInt(m_Gold);
		Text_Gold->SetText(FText::FromString(strGold));
	}

	if(Text_Name)
	{
		Text_Name->SetText(FText::FromString(strName));
	}

	if(m_LevelNumber >= g_CSDefineRecordMgr->GetValue_Number("GV_CHARACTERSTAT_MAX"))
	{
		if(Text_Gold)
		{
			Text_Gold->SetText(FText::FromString(TEXT("Max")));
		}
		if(SlotButton)
		{
			SlotButton->SetIsEnabled(false);
		}
	}
	else
	{
		if(Text_Gold)
		{
			FString strGold = FString::FromInt(m_Gold);
			Text_Gold->SetText(FText::FromString(strGold));
		}
	}
	
	if(Text_Stat)
	{
		FString strStat;
		if(m_eCharacterValue == nCharacterStatValue::Decline)
		{
			strStat =TEXT("-")+ FString::SanitizeFloat(m_fStat);
		}
		else
		{
		strStat =TEXT("+")+ FString::SanitizeFloat(m_fStat);
		}
	

		if(m_eCharacterValue == nCharacterStatValue::Percent || m_eCharacterValue == nCharacterStatValue::Decline)
		{
			strStat += TEXT("%");
		}
		Text_Stat->SetText(FText::FromString(strStat));
	}


		if(SlotButton)
		{
			SlotButton->SetIsEnabled(m_Gold > g_PSCharacterMgr->GetGold() ? false : true);
		}
	
}

void UPSWidgetSlot_CharacterStatStore::RefreshStat()
{
	if(g_PSCharacterMgrValid)
	{
		FString strStatTid;
		if(!strName.IsEmpty())
		{
			m_eCharacterStat = nCharacterStat::GetEnum(strTid,false);
			m_LevelNumber = g_PSCharacterMgr->GetLevel(strTid);
		}

		strStatTid = strTid + TEXT("_") + FString::FromInt(m_LevelNumber);

		if(PSCharacterStatDetailRecord* pRecord = g_CharacterStatDetailRecordMgr->FindRecord(strStatTid))
		{
			m_fStat = pRecord->m_fValue;
			m_Gold = pRecord->m_Gold;
			m_eCharacterValue = pRecord->m_eCharacterStateValue;
		}
	}
	RefreshText();
}

void UPSWidgetSlot_CharacterStatStore::SellStat(int32 _gold)
{
	if(_gold >= m_Gold)
	{
		int32 subGold = _gold - m_Gold;

		if(g_PSCharacterMgrValid)
		{
			g_PSCharacterMgr->SetGold(subGold);
		}
		m_LevelNumber++;
		
		if(g_PSCharacterMgrValid)
		{
			g_PSCharacterMgr->SetStatValue(strTid,m_LevelNumber);
			g_PSCharacterMgr->InitCharacterStat();
			g_DataBaseMgr->SetCharacterStats(strTid,m_LevelNumber);
		}
	}
}
