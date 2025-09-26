// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_Stage.h"

#include "PSScrollWidget_Stage.h"
#include "PSWidget_StageInfo.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/DataTable/DataManager/PSClearBonusRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSClearBonusRecord.h"
#include "ProtoSurvivor/Manager/PSAdMobMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "ProtoSurvivor/UI/Play/PSWidget_Sound.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "ProtoSurvivor/UI/Play/PSWidget_AdRemove.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"

void UPSWidget_Stage::InitData()
{
	Super::InitData();

	if (g_DataBaseMgrValid)
	{
		if (g_DataBaseMgr->GetIsLoad())
		{
			InitStageWidget();
		}
	}

	if (SoundWidget)
	{
		SoundWidget->Hide();
	}
	if (AdRemoveWidget)
	{
		AdRemoveWidget->Hide();
	}
}

void UPSWidget_Stage::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Start, &UPSWidget_Stage::OnClicked_Start);
	BIND_BUTTON_EVENT(Button_Next, &UPSWidget_Stage::OnClicked_Next);
	BIND_BUTTON_EVENT(Button_Previous, &UPSWidget_Stage::OnClicked_Previous);
	BIND_BUTTON_EVENT(Button_Sound, &UPSWidget_Stage::UPSWidget_Stage::OnClicked_Sound);
	BIND_BUTTON_EVENT(Button_AdRemove, &UPSWidget_Stage::OnClicked_AdRemove);
	BIND_BUTTON_EVENT(Button_Bonus,&UPSWidget_Stage::OnClicked_Bonus);
	BIND_BUTTON_EVENT(Button_First, &UPSWidget_Stage::OnClicked_First);
	BIND_BUTTON_EVENT(Button_Last, &UPSWidget_Stage::OnClicked_Last);

	if (g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this, &UPSWidget_Stage::InitStageWidget);
	}

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.AddUObject(this, &UPSWidget_Stage::ApplyAdRemove);
	}
}

void UPSWidget_Stage::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Start);
	UNBIND_BUTTON_EVENT_ALL(Button_Next);
	UNBIND_BUTTON_EVENT_ALL(Button_Previous);
	UNBIND_BUTTON_EVENT_ALL(Button_Sound);
	UNBIND_BUTTON_EVENT_ALL(Button_AdRemove);
	UNBIND_BUTTON_EVENT_ALL(Button_Bonus);
	UNBIND_BUTTON_EVENT_ALL(Button_First);
	UNBIND_BUTTON_EVENT_ALL(Button_Last);

	if (g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
	}

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}



void UPSWidget_Stage::OnClicked_Start()
{
	bool isAllEquip = IsAllEquip();

	if(!isAllEquip)
	{
		g_GameMsgMgr->ShowMessageBox("Popup_Not_AllEquip");
		return;
	}

	if(g_PSCharacterMgrValid)
	{
		float UseEnergy = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_ENERGY_START");
		int32 currentEnergy = g_PSCharacterMgr->GetEnergy();
		if(currentEnergy < UseEnergy)
		{
			return;
		}
		
		currentEnergy -= UseEnergy;
		g_PSCharacterMgr->SetEnergy(currentEnergy);
	}
	g_PSAdMobMgr->LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
	if (m_iCurrentStage <= m_iMaxOpenStage)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
	}
}

void UPSWidget_Stage::OnClicked_Next()
{
	if (g_StageMgrValid)
	{
		SetCurrentStage(m_iCurrentStage+1);
		g_StageMgr->SetStageNumber(m_iCurrentStage);
	}
}

void UPSWidget_Stage::OnClicked_Previous()
{
	if (g_StageMgrValid)
	{
		SetCurrentStage(m_iCurrentStage-1);
		g_StageMgr->SetStageNumber(m_iCurrentStage);
	}
}

void UPSWidget_Stage::OnClicked_Sound()
{
	if (SoundWidget)
	{
		SoundWidget->Show();
	}
}

void UPSWidget_Stage::OnClicked_AdRemove()
{
	if (AdRemoveWidget)
	{
		AdRemoveWidget->Show();
	}
}

void UPSWidget_Stage::OnClicked_Bonus()
{


		if(Widget_StageInfo)
		{
			Widget_StageInfo->CreateBonusInfo(m_iCurrentStage);
			Widget_StageInfo->Show();
		}
	
}

void UPSWidget_Stage::OnClicked_First()
{
	SetCurrentStage(1);
	g_StageMgr->SetStageNumber(m_iCurrentStage);
}

void UPSWidget_Stage::OnClicked_Last()
{
	SetCurrentStage(m_iMaxOpenStage);
	g_StageMgr->SetStageNumber(m_iCurrentStage);
}

void UPSWidget_Stage::SetStageText()
{
	if (g_StageMgr->m_arrStageRecord.Num() >= m_iCurrentStage)
	{
		FString stageName = g_StageMgr->m_arrStageRecord[m_iCurrentStage-1]->m_strStageName;
		StageNameText->SetText(FText::FromString(FString::Printf(TEXT("스테이지 %i: %s"), m_iCurrentStage, *stageName)));

		int32 highWave = 0;
		if (g_StageMgr->m_arrStageHighWave.Num() >= m_iCurrentStage)
		{
			FString stageKey = "Stage_" + FString::FromInt(m_iCurrentStage);
			highWave = g_StageMgr->m_arrStageHighWave[stageKey];
		}
		HighWaveText->SetText(FText::FromString(FString::Printf(TEXT("최고 웨이브 클리어: %d"), highWave)));
	}
}

void UPSWidget_Stage::SetCurrentStage(int32 CurrentStage)
{
	m_iCurrentStage = CurrentStage;
	if (ScrollBox_Stage)
	{
		ScrollBox_Stage->ScrollToSlot(m_iCurrentStage - 1);
	}
	SetStageText();
	setButtonEnabled();
}

bool UPSWidget_Stage::IsAllEquip()
{
	bool CheckAllEquip = true;
	TArray<FString> arrWeaponKeys;
	g_PSCharacterMgr->m_MapWeapon.GetKeys(arrWeaponKeys);

	if(g_PSCharacterMgr->m_mapEquipWeapon.Num()<8)
	{
		for(FString strWeapon : arrWeaponKeys)
		{
			if(g_PSCharacterMgr->m_MapWeapon.Contains(strWeapon))
			{
				if(FWeaponStat* pWeaponStat = g_PSCharacterMgr->m_MapWeapon[strWeapon])
				{
					if(pWeaponStat->m_IsLock)
					{
						if(!pWeaponStat->m_IsEquip)
						{
							CheckAllEquip = false;
							
							break;
						}
					}
				}
			}
		}
	}
	return CheckAllEquip;
}

void UPSWidget_Stage::InitStageWidget()
{
	TArray<PSStageRecord*> arrStageRecord = g_StageMgr->m_arrStageRecord;
	if (arrStageRecord.Num() > 0)
	{
		m_iMaxStage = arrStageRecord.Num();
	}
	
	if (g_StageMgrValid)
	{
		m_iMaxOpenStage = g_StageMgr->GetMaxStageNumber();
		SetCurrentStage(g_StageMgr->GetStageNumber());
	}

	if (g_PSCharacterMgrValid)
	{
		if (g_PSCharacterMgr->GetPurchasedAdRemove())
		{
			if (Button_AdRemove)
			{
				Button_AdRemove->SetIsEnabled(false);
			}
		}
	}
}

void UPSWidget_Stage::ApplyAdRemove()
{
	if (AdRemoveWidget)
	{
		AdRemoveWidget->Hide();
	}
	if (Button_AdRemove)
	{
		Button_AdRemove->SetIsEnabled(false);
	}
}

void UPSWidget_Stage::setButtonEnabled()
{
	bool buttonEnabled = m_iCurrentStage > m_iMaxOpenStage;
	if (Button_Start)
	{
		Button_Start->SetIsEnabled(!buttonEnabled);
	}

	int32 stageOpenNumber = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_OPEN_NUMBER");
	buttonEnabled = (m_iCurrentStage < m_iMaxOpenStage + stageOpenNumber) && (m_iCurrentStage < m_iMaxStage);
	if (Button_Next)
	{
		Button_Next->SetIsEnabled(buttonEnabled);
	}

	buttonEnabled = m_iCurrentStage > 1;
	if (Button_Previous)
	{
		Button_Previous->SetIsEnabled(buttonEnabled);
	}
}
