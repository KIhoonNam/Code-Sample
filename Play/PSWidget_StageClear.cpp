// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_StageClear.h"

#include "PSWidgetSlot_StageClear.h"
#include "Components/UniformGridSlot.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/DataTable/DataManager/PSClearBonusRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSClearBonusRecord.h"
#include "ProtoSurvivor/Manager/PSAdMobMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "NiagaraSystemWidget.h"

void UPSWidget_StageClear::InitData()
{
	Super::InitData();

	if (g_PSCharacterMgrValid)
	{
		if (g_PSCharacterMgr->GetPurchasedAdRemove())
		{
			SetAdRemove();
		}
	}
}

void UPSWidget_StageClear::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UPSWidget_StageClear::ReleaseData()
{
	Super::ReleaseData();
}

void UPSWidget_StageClear::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_End,&UPSWidget_StageClear::OnClickEnd);
	BIND_BUTTON_EVENT(Button_Reward,&UPSWidget_StageClear::OnClickReward);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.AddUObject(this, &UPSWidget_StageClear::SetAdRemove);
	}
}

void UPSWidget_StageClear::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_End);
	UNBIND_BUTTON_EVENT_ALL(Button_Reward);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidget_StageClear::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (g_StageMgrValid)
	{
		g_StageMgr->SetIsGameEnded(true);
		if (g_PSPauseMgrValid)
		{
			g_PSPauseMgr->AddPauseReason(GetWorld(), nPauseType::Clear);
		}
	}
	
	SetStageClear();
}

void UPSWidget_StageClear::SetStageClear()
{
	if(g_StageMgrValid)
	{
		if(Grid_Result)
		{
			Grid_Result->ClearChildren();
			FString strStageNumber = FString::FromInt(g_StageMgr->GetStageNumber());

			int32 _count = 0;
			if(PSClearBonusRecord* pRecord = g_ClearBonusRecordMgr->FindRecord(strStageNumber))
			{
				CreateGold(pRecord,&_count);
				CreatePieceWeapon(pRecord,&_count);
				CreateUnlockWeapon(pRecord,&_count);
				SetSlotSort();
				if(g_PSPropertiesMgrValid)
				{
					g_PSPropertiesMgr->ResetGlobalBuff();
					g_PSPropertiesMgr->ResetWeaponBuff();
				}
			}
			Button_Reward->SetVisibility(Grid_Result->GetChildrenCount() == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
		bool bResult = g_StageMgr->GetIsClear();
		if(Text_Result)
		{
		
			FString strResult = bResult ? TEXT("성공") : TEXT("실패");
			Text_Result->SetText(FText::FromString(strResult));
		}

		if(Image_Result)
		{
			FString strSPRResult = bResult ? TEXT("SPR_Result_Success") : TEXT("SPR_Result_Failed");
			Image_Result->SetBrushFromSpriteName(strSPRResult);
		}

		if(Image_label)
		{
			FString strSPRResult = bResult ? TEXT("Label_Ribbon01_LightGreen_Sprite") : TEXT("Label_Ribbon01_Orange_Sprite");
			Image_label->SetBrushFromSpriteName(strSPRResult);
		}

		if(g_SoundMgrValid)
		{
			FString strSoundResult = bResult ? TEXT("Sound_StageClear_Cue") : TEXT("Sound_StageFailed_Cue");
			g_SoundMgr->PlaySound2D(strSoundResult);
		}

		if(Niagara_Fanfare && Niagara_Star)
		{
			bResult ? Niagara_Fanfare->ActivateSystem(true) : Niagara_Fanfare->DeactivateSystem();
			bResult ? Niagara_Star->ActivateSystem(true) : Niagara_Star->DeactivateSystem();
		}
	}
}

void UPSWidget_StageClear::CreateGold(PSClearBonusRecord* pRecord, int32* _count)
{
    if (Grid_Result)
    {
        int32 columnMax = g_CSDefineRecordMgr->GetValue_Number("GV_EQUIPMENT_Column");
        float fGold = pRecord->m_GoldNumber * ((float)g_StageMgr->GetWaveNumber() / g_StageMgr->GetMaxWaveNumber());

        if (fGold <= 0)
        {
            return;
        }

        if (UPSWidgetSlot_StageClear* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_StageClear>(TEXT("S_StageClear_BP"), GetWorld()))
        {
            int32 column = *_count % columnMax;
            int32 rows = *_count / columnMax;

            Grid_Result->AddChildToUniformGrid(pSlot, rows, column);
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


void UPSWidget_StageClear::CreatePieceWeapon(PSClearBonusRecord* pRecord,int32* _count)
{
	if(Grid_Result)
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
				amount = amount * ((float)g_StageMgr->GetWaveNumber() / g_StageMgr->GetMaxWaveNumber());
				
				if (amount <= 0)
				{
					continue;
				}
				
				if(UPSWidgetSlot_StageClear* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_StageClear>(TEXT("S_StageClear_BP"),GetWorld()))
				{
					int32 column = *_count %columnMax;
					int32 rows = *_count / columnMax;

					Grid_Result->AddChildToUniformGrid(pSlot,rows,column);
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

void UPSWidget_StageClear::CreateUnlockWeapon(PSClearBonusRecord* pRecord,int32* _count)
{
	if(Grid_Result)
	{
		if( (g_StageMgr->GetWaveNumber() / g_StageMgr->GetMaxWaveNumber()) == 1)
		{
			int32 columnMax = g_CSDefineRecordMgr->GetValue_Number("GV_EQUIPMENT_Column");
			for(FString strUnLockWeapon : pRecord->m_arrUnLockWeapon)
			{
				TArray<FString> arrUnlockWeapon;
				strUnLockWeapon.ParseIntoArray(arrUnlockWeapon,TEXT(":"));
				if(arrUnlockWeapon.IsValidIndex(0))
				{
					FString strWeaponTid = arrUnlockWeapon[0];
					if(g_PSCharacterMgr->m_MapWeapon.Contains(strWeaponTid))
					{
						if(g_PSCharacterMgr->m_MapWeapon[strWeaponTid]->m_IsLock) continue;
						int32 amount = 1;
						if(UPSWidgetSlot_StageClear* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_StageClear>(TEXT("S_StageClear_BP"),GetWorld()))
						{
							int32 column = *_count %columnMax;
							int32 rows = *_count / columnMax;

							Grid_Result->AddChildToUniformGrid(pSlot,rows,column);
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
	}
}

void UPSWidget_StageClear::SetSlotSort()
{

}

void UPSWidget_StageClear::SetAdRemove()
{
	if (AdIconImage)
	{
		AdIconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPSWidget_StageClear::OnClickEnd()
{
	//g_UIMgr->HideUIPanel(nUIPanelType::StageClear);

	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->ClearAllPauseReasons(GetWorld());
	}

	if(Grid_Result)
	{
		for(UWidget* pWidget : Grid_Result->GetAllChildren())
		{
			if(UPSWidgetSlot_StageClear* pSlot = Cast<UPSWidgetSlot_StageClear>(pWidget))
			{
				pSlot->GetClearBonus(1);
			}
		}
	}

	UGameplayStatics::OpenLevel(GetWorld(),"Level_Lobby");
}

void UPSWidget_StageClear::OnClickReward()
{
	if (g_PSAdMobMgrValid)
	{
		g_PSAdMobMgr->ShowRewardAd_StageClear(0, "StageClear", this);
	}
}

void UPSWidget_StageClear::GetRewardBonus()
{
	if(Grid_Result)
	{
		for(UWidget* pWidget : Grid_Result->GetAllChildren())
		{
			if(UPSWidgetSlot_StageClear* pSlot = Cast<UPSWidgetSlot_StageClear>(pWidget))
			{
				pSlot->GetClearBonus(2);
			}
		}
	}
}


