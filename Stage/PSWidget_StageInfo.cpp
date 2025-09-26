// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_StageInfo.h"

#include "Components/UniformGridSlot.h"
#include "ProtoSurvivor/DataTable/DataManager/PSClearBonusRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataManager/PSStageRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSClearBonusRecord.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSStageRecord.h"
#include "ProtoSurvivor/UI/Play/PSWidgetSlot_StageClear.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"

void UPSWidget_StageInfo::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_SingleOk,&UPSWidget_StageInfo::OnClicked_OK);
}

void UPSWidget_StageInfo::UnLinkEvent()
{

	UNBIND_BUTTON_EVENT_ALL(Button_SingleOk);
	Super::UnLinkEvent();
}

void UPSWidget_StageInfo::CreateBonusInfo(int32 _stageNumber)
{

		FString strTid = FString::FromInt(_stageNumber);
		int32 _count = 0;
		if(PSClearBonusRecord* pRecord = g_ClearBonusRecordMgr->FindRecord(strTid))
		{
			if(Grid_BonusInfo)
			{
				Grid_BonusInfo->ClearChildren();
			}
			CreateGold(pRecord,&_count);
			CreatePieceWeapon(pRecord,&_count);
			CreateUnlockWeapon(pRecord,&_count);
		}

		if(PSStageRecord* pStageRecord = g_StageRecordMgr->FindRecord(strTid))
		{
			if(Text_Stage)
			{
				FString strStageName = TEXT("스테이지 ") + FString::FromInt(_stageNumber) + TEXT(": ")+pStageRecord->m_strStageName;
				Text_Stage->SetText(FText::FromString(strStageName));
			}
		}
	
}

void UPSWidget_StageInfo::CreateGold(PSClearBonusRecord* pRecord, int32* _count)
{
	if (Grid_BonusInfo)
	{
		int32 columnMax = g_CSDefineRecordMgr->GetValue_Number("GV_EQUIPMENT_Column");
		float fGold = pRecord->m_GoldNumber;

		if (fGold <= 0)
		{
			return;
		}

		if (UPSWidgetSlot_StageClear* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_StageClear>(TEXT("S_StageClear_BP"), GetWorld()))
		{
			int32 column = *_count % columnMax;
			int32 rows = *_count / columnMax;

			Grid_BonusInfo->AddChildToUniformGrid(pSlot, rows, column);
			if (UUniformGridSlot* pUniformSLot = Cast<UUniformGridSlot>(pSlot->Slot))
			{
				pUniformSLot->SetHorizontalAlignment(HAlign_Center);
			}

			pSlot->SetGold(fGold);
			pSlot->SetClearType(nClearType::Gold);
			*_count += 1;
		}
	}
}

void UPSWidget_StageInfo::CreatePieceWeapon(PSClearBonusRecord* pRecord, int32* _count)
{
	if(Grid_BonusInfo)
	{
		int32 columnMax = g_CSDefineRecordMgr->GetValue_Number("GV_EQUIPMENT_Column");
		for(FString strPieceWeapon : pRecord->m_arrWeaponPiece)
		{
			TArray<FString> arrPieceWeapon;
			strPieceWeapon.ParseIntoArray(arrPieceWeapon,TEXT(":"));
			if(arrPieceWeapon.Num()>=2)
			{
				FString strWeaponTid = arrPieceWeapon[0];
				int32 amount = FCString::Atoi(*arrPieceWeapon[1]);
				
				if (amount <= 0)
				{
					continue;
				}
				
				if(UPSWidgetSlot_StageClear* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_StageClear>(TEXT("S_StageClear_BP"),GetWorld()))
				{
					int32 column = *_count %columnMax;
					int32 rows = *_count / columnMax;

					Grid_BonusInfo->AddChildToUniformGrid(pSlot,rows,column);
					if(UUniformGridSlot* pUniformSLot = Cast<UUniformGridSlot>(pSlot->Slot))
					{
						pUniformSLot->SetHorizontalAlignment(HAlign_Center);
					}
					pSlot->SetPieceWeapon(strWeaponTid,amount);
					pSlot->SetClearType(nClearType::Piece);
					*_count += 1;
				}
			}
		}
	}
}

void UPSWidget_StageInfo::CreateUnlockWeapon(PSClearBonusRecord* pRecord, int32* _count)
{
	if(Grid_BonusInfo)
	{
		int32 columnMax = g_CSDefineRecordMgr->GetValue_Number("GV_EQUIPMENT_Column");
		for(FString strUnLockWeapon : pRecord->m_arrUnLockWeapon)
		{
			TArray<FString> arrUnlockWeapon;
			strUnLockWeapon.ParseIntoArray(arrUnlockWeapon,TEXT(":"));
			if(arrUnlockWeapon.IsValidIndex(0))
			{
				FString strWeaponTid = arrUnlockWeapon[0];

				int32 amount = 1;
				if(UPSWidgetSlot_StageClear* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_StageClear>(TEXT("S_StageClear_BP"),GetWorld()))
				{
					int32 column = *_count %columnMax;
					int32 rows = *_count / columnMax;

					Grid_BonusInfo->AddChildToUniformGrid(pSlot,rows,column);
					if(UUniformGridSlot* pUniformSLot = Cast<UUniformGridSlot>(pSlot->Slot))
					{
						pUniformSLot->SetHorizontalAlignment(HAlign_Center);
					}
					pSlot->SetPieceWeapon(strWeaponTid,amount);
					pSlot->SetClearType(nClearType::Unlock);
					*_count += 1;
				}
			}
		}
	}
}

void UPSWidget_StageInfo::OnClicked_OK()
{
	Hide();
}
