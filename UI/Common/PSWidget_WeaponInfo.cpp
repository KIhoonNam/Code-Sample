// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_WeaponInfo.h"

#include "PSWidgetSlot_WeaponTalent.h"
#include "PSWidget_WeaponInfoDesc.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBoxSlot.h"
#include "ProtoSurvivor/DataTable/DataManager/PSPropertiesRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponPieceRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSPropertiesRecord.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "ProtoSurvivor/UI/Lobby/PSWidget_SignalMark.h"

void UPSWidget_WeaponInfo::InitData()
{
	Super::InitData();
	
}

void UPSWidget_WeaponInfo::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Close,&UPSWidget_WeaponInfo::OnClick_Close);
	BIND_BUTTON_EVENT(Button_LevelUp,&UPSWidget_WeaponInfo::OnClick_LevelUp);
}

void UPSWidget_WeaponInfo::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Close);
	UNBIND_BUTTON_EVENT_ALL(Button_LevelUp);
	Super::UnLinkEvent();
}

void UPSWidget_WeaponInfo::SetWeaponInfo(FWeaponStat* pStat)
{
	if(pStat)
	{
		m_WeaponStat= pStat;
		m_eRateType = pStat->m_eRateType;
		m_strWeaponTid = pStat->m_strTid;
		m_Piece = pStat->Piece;
		if(Image_Weapon)
		{
			Image_Weapon->SetBrushFromSpriteName(pStat->m_strSpriteName);
		}

		if(Image_Rate)
		{
			Image_Rate->SetBrushFromSpriteName(pStat->m_strRateSpriteName + TEXT("_Long"));
		}
	
		if(Text_WeaponName)
		{
			Text_WeaponName->SetText(FText::FromString(pStat->m_strName));
		}


		
		FString CurrentLevelName = GetWorld()->GetMapName();

		if (CurrentLevelName.Contains("Level_Lobby"))
		{
			if(Button_LevelUp)
			{
				Button_LevelUp->SetVisibility(ESlateVisibility::Visible);
			}
			if(Text_Level)
			{
				m_Level = pStat->Level;
				FString strLevel = TEXT("Lv.") + FString::FromInt(m_Level);
				Text_Level->SetText(FText::FromString(strLevel));
			}
			SetWeaponLevelUp();
			SetWeaponDesc();
			SetWeaponTalent(pStat);
			if(m_WeaponStat)
			{
				UpdateMark(m_WeaponStat);
			}
		}
		else if (CurrentLevelName.Contains("NewMap"))
		{
			if(Text_Level)
			{
				m_Level = pStat->UpgradeLevel;
				FString strLevel = TEXT("합성 레벨 : ") + FString::FromInt(m_Level);
				Text_Level->SetText(FText::FromString(strLevel));
			}
			if(Text_Piece)
			{
				FString strMaxLevel = TEXT("MAX : ") + FString::FromInt(pStat->MaxUpgradeLevel);
				Text_Piece->SetText(FText::FromString(strMaxLevel));
			}
			if(Button_LevelUp)
			{
				Button_LevelUp->SetVisibility(ESlateVisibility::Collapsed);
			}
			if(ProgressBar_Piece && W_SignalMark_BP)
			{
				ProgressBar_Piece->SetVisibility(ESlateVisibility::Collapsed);
				W_SignalMark_BP->SetVisibility(ESlateVisibility::Collapsed);
			}
			SetWeaponDesc();
			SetInGameWeaponTalent(pStat);
		}
		
	}
}

void UPSWidget_WeaponInfo::SetWeaponTalent(FWeaponStat* pStat)
{
	if(pStat)
	{
		TArray<FString> arrWeaponTalent;

		pStat->m_strWeaponTalnet.ParseIntoArray(arrWeaponTalent,TEXT(","));
		
		ScrollBox_Talent->ClearChildren();
		if(ScrollBox_Talent)
		{
			for(const FString& strWeaponTalent:arrWeaponTalent)
			{
				if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord(strWeaponTalent))
				{
					if(UPSWidgetSlot_WeaponTalent* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_WeaponTalent>("S_WeaponTalent_BP",GetWorld()))
					{
						ScrollBox_Talent->AddChild(pSlot);

						if(UScrollBoxSlot* pScrollSlot = Cast<UScrollBoxSlot>(pSlot->Slot))
						{
							pScrollSlot->SetPadding(FMargin(0,10,0,10));
							pScrollSlot->SetHorizontalAlignment(HAlign_Fill);
							pScrollSlot->SetVerticalAlignment(VAlign_Fill);
						}

						int32 RoundedValue = FMath::RoundToInt(pRecord->m_Properties->m_fValue);
						int32 RoundedTriggerValue = FMath::RoundToInt(pRecord->m_Properties->m_fTriggerValue);
						
						FString ModifiedScript = FString::Printf(TEXT("Lv.%d : "), pRecord->m_Properties->m_Level) +
							ReplacePlaceholders( pRecord->m_Properties->m_strScript,
							FString::FromInt(RoundedValue),
							FString::FromInt(RoundedTriggerValue));

						bool lock = false;
						pStat->Level >= pRecord->m_Properties->m_Level ? lock = true : lock =false;
						pSlot->SetEnableTalent(lock,ModifiedScript,strWeaponTalent);
					}
				}
			}

			ScrollBox_Talent->ScrollToStart();
		}
	}
}

void UPSWidget_WeaponInfo::SetInGameWeaponTalent(FWeaponStat* pStat)
{
    if (!pStat || !ScrollBox_Talent || !g_PSCharacterMgr || g_PSCharacterMgr->m_mapEquipWeapon.Num() == 0) return;

    TArray<FString> arrWeaponTalent;
    pStat->m_strWeaponTalnet.ParseIntoArray(arrWeaponTalent, TEXT(","));
    ScrollBox_Talent->ClearChildren();

    // 현재 사용 중인 무기 찾기
    for (const TPair<int32, FWeaponStat*>& Pair : g_PSCharacterMgr->m_mapEquipWeapon)
    {
        if (Pair.Value && Pair.Value->m_strTid == pStat->m_strTid)
        {
        	for(FString tid : pStat->m_arrCurrentTalent)
        	{
        		if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord(tid))
        		{
        			AddTalentToUI(pRecord, tid);
        		}
        	}
            // 값 기반 버프 처리
            // HandleBuffType(Pair.Value->m_fDamageBuff, arrWeaponTalent, nBuffType::Damage, 1.0f);
            // HandleBuffType(Pair.Value->m_fCoolTimeBuff, arrWeaponTalent, nBuffType::CoolTime, 1.0f);
            // HandleBuffType(Pair.Value->m_fCriticalPercentBuff, arrWeaponTalent, nBuffType::Critical, 0.0f);
            // HandleBuffType(Pair.Value->m_fCriticalDamageBuff, arrWeaponTalent, nBuffType::CriticalDamage, 1.0f);
            // HandleBuffType(Pair.Value->m_fArmorBuff, arrWeaponTalent, nBuffType::Armor, 1.0f);
            // HandleBuffType(Pair.Value->m_fHPBuff, arrWeaponTalent, nBuffType::HP, 1.0f);
            // HandleBuffType(Pair.Value->m_fHealBuff, arrWeaponTalent, nBuffType::Heal, 1.0f);
            //
            // // 속성 기반 버프 처리
            // HandleAttributeBuff(Pair.Value, arrWeaponTalent);
        }
    }

    ScrollBox_Talent->ScrollToStart();
}

void UPSWidget_WeaponInfo::HandleBuffType(float BuffValue, const TArray<FString>& arrWeaponTalent, nBuffType::en BuffType, float DefaultValue)
{
    if (BuffValue == DefaultValue) return; // 기본 값이면 처리하지 않음

    for (const FString& strWeaponTalent : arrWeaponTalent)
    {
        if (PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord(strWeaponTalent))
        {
            if (pRecord->m_Properties->m_eBuffType == BuffType)
            {
            	FString ModifiedScript = ReplacePlaceholders( strWeaponTalent,
			FString::SanitizeFloat(pRecord->m_Properties->m_fValue),
			FString::SanitizeFloat(pRecord->m_Properties->m_fTriggerValue));
                AddTalentToUI(pRecord, strWeaponTalent);
            }
        }
    }
}

void UPSWidget_WeaponInfo::HandleAttributeBuff(FWeaponStat* Weapon, const TArray<FString>& arrWeaponTalent)
{

}

void UPSWidget_WeaponInfo::AddTalentToUI(PSPropertiesRecord* pRecord, const FString& strWeaponTalent)
{
	if (UPSWidgetSlot_WeaponTalent* pSlot = CSUIUtils::LoadWidget<UPSWidgetSlot_WeaponTalent>("S_WeaponTalent_BP", GetWorld()))
	{
		ScrollBox_Talent->AddChild(pSlot);

		if (UScrollBoxSlot* pScrollSlot = Cast<UScrollBoxSlot>(pSlot->Slot))
		{
			pScrollSlot->SetPadding(FMargin(0, 10, 0, 10));
			pScrollSlot->SetHorizontalAlignment(HAlign_Fill);
			pScrollSlot->SetVerticalAlignment(VAlign_Fill);
		}
		
		int32 RoundedValue = FMath::RoundToInt(pRecord->m_Properties->m_fValue);
		int32 RoundedTriggerValue = FMath::RoundToInt(pRecord->m_Properties->m_fTriggerValue);
		
		FString ModifiedScript = ReplacePlaceholders( pRecord->m_Properties->m_strScript,
		FString::FromInt(RoundedValue),
		FString::FromInt(RoundedTriggerValue));

		pSlot->SetEnableTalent(true, ModifiedScript, strWeaponTalent);
	}
}

void UPSWidget_WeaponInfo::SetWeaponLevelUp()
{
	if(Button_LevelUp)
	{

		ESlateVisibility SlateVisibility = m_WeaponStat->m_IsLock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		Button_LevelUp->SetVisibility(SlateVisibility);
		if(PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(m_Level)))
		{
			if(pRecord->m_mapPiece.Contains(m_eRateType))
			{
				if(Text_Piece)
				{
					m_MaxPiece = pRecord->m_mapPiece[m_eRateType];
					FString strPiece = FString::FromInt(m_Piece) + TEXT("/") + FString::FromInt(m_MaxPiece);
					Text_Piece->SetText(FText::FromString(strPiece));
				}
			}

			if(pRecord->m_mapGold.Contains(m_eRateType))
			{
				if(Text_Gold)
				{
					m_MaxGold= pRecord->m_mapGold[m_eRateType];
					int32 currentGold = g_PSCharacterMgr->GetGold();
					FString strGold = FString::FromInt(m_MaxGold);
					Text_Gold->SetText(FText::FromString(strGold));
					
					bool buttonEnable = true;
					if(currentGold < m_MaxGold )
					{
						buttonEnable = false;
					}
					if(m_Piece < m_MaxPiece)
					{
						buttonEnable = false;
					}
					Button_LevelUp->SetIsEnabled(buttonEnable);
				}
			}
		}
		else
		{
			if(Text_Piece)
			{
				ProgressBar_Piece->SetPercent(0.0f);
				FString strPiece = TEXT("0/1");
				Text_Piece->SetText(FText::FromString(strPiece));
			}
		}
	}
}

void UPSWidget_WeaponInfo::SetWeaponDesc()
{
	if(m_WeaponStat)
	{
		nTargetType::en eTargetType = m_WeaponStat->m_eTargetType;
		FString strBPName;
		TArray<FString> arrStrDesc;

		arrStrDesc.Emplace(m_WeaponStat->m_strTargetDesc);
		arrStrDesc.Emplace(FString::SanitizeFloat(m_WeaponStat->m_fCoolTime));
		switch (eTargetType)
		{
		case nTargetType::HP:
			{
				strBPName = TEXT("W_WeaponInfoHeal_BP");
				float health = (m_WeaponStat->UpgradeLevel != 1) ? m_WeaponStat->m_fHP : m_WeaponStat->m_fHP + (m_WeaponStat->m_fLevelHealth * (m_WeaponStat->Level-1));
				float heal= (m_WeaponStat->UpgradeLevel != 1) ? m_WeaponStat->m_fHeal : m_WeaponStat->m_fHeal + (m_WeaponStat->m_fLevelDamage * (m_WeaponStat->Level-1));
				arrStrDesc.Emplace(FString::FromInt(health));
				arrStrDesc.Emplace(FString::FromInt(heal));
			}break;
		case nTargetType::Armor:
			{
				strBPName = TEXT("W_WeaponInfoArmor_BP");
				float health = (m_WeaponStat->UpgradeLevel != 1) ? m_WeaponStat->m_fHP : m_WeaponStat->m_fHP + (m_WeaponStat->m_fLevelHealth * (m_WeaponStat->Level-1));
				float armor= (m_WeaponStat->UpgradeLevel != 1) ? m_WeaponStat->m_fArmor : m_WeaponStat->m_fArmor + (m_WeaponStat->m_fLevelDamage * (m_WeaponStat->Level-1));
				arrStrDesc.Emplace(FString::FromInt((int32)health));
				arrStrDesc.Emplace(FString::FromInt(armor));
			}break;
		default:
			{
				float damage = (m_WeaponStat->UpgradeLevel != 1) ? m_WeaponStat->m_fDamage : m_WeaponStat->GetLevelDamage();
				strBPName = TEXT("W_WeaponInfoAttack_BP");
				arrStrDesc.Emplace(FString::FromInt(damage));
				arrStrDesc.Emplace(FString::FromInt(m_WeaponStat->m_fRange));
			}
			break;
		}

		if(UPSWidget_WeaponInfoDesc* pInfoDesc = CSUIUtils::LoadWidget<UPSWidget_WeaponInfoDesc>(*strBPName,GetWorld()))
		{
			if(Overlay_InfoDesc)
			{
				Overlay_InfoDesc->ClearChildren();
				Overlay_InfoDesc->AddChildToOverlay(pInfoDesc);

				if(UOverlaySlot* pSlot = Cast<UOverlaySlot>(pInfoDesc->Slot))
				{
					pSlot->SetHorizontalAlignment(HAlign_Fill);
					pSlot->SetVerticalAlignment(VAlign_Fill);
				}
			}
			pInfoDesc->SettingInfoDesc(arrStrDesc);
		}
	}
}

void UPSWidget_WeaponInfo::RefreshTalent()
{
	if(ScrollBox_Talent)
	{
		for(UCSWidgetSlot* pSlot : ScrollBox_Talent->GetWidgetChildren())
		{
			if(UPSWidgetSlot_WeaponTalent* pTalentSlot = Cast<UPSWidgetSlot_WeaponTalent>(pSlot))
			{
				FString tid = pTalentSlot->GetTalentTid();

				if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord(tid))
				{
					if(m_Level >= pRecord->m_Properties->m_Level)
					{
						pTalentSlot->RefreshEnableTalent();
					}
				}
			}
		}
	}
}
void UPSWidget_WeaponInfo::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);


	SetWeaponInfo(g_PSCharacterMgr->m_pWeaponStat);
}

void UPSWidget_WeaponInfo::OnClick_Close()
{
	UE_LOG(LogTemp,Warning,TEXT("Click Close"))


	Hide();
}

void UPSWidget_WeaponInfo::OnClick_LevelUp()
{
	UE_LOG(LogTemp,Warning,TEXT("Click Levelup"))

	if(m_Piece >= m_MaxPiece)
	{
		int32 currentGold = g_PSCharacterMgr->GetGold();
		if(currentGold>=m_MaxGold)
		{
			currentGold -= m_MaxGold;
			g_PSCharacterMgr->SetGold(currentGold);
			m_Level++;
			m_Piece -= m_MaxPiece;

			if(g_DataBaseMgrValid)
			{
				g_DataBaseMgr->SetWeaponLevel(m_strWeaponTid,m_Level,m_Piece);
			}

			if(Text_Level)
			{
				FString strLevel = TEXT("Lv.") + FString::FromInt(m_Level);
				Text_Level->SetText(FText::FromString(strLevel));
			}

			if(m_WeaponStat)
			{
				m_WeaponStat->Level = m_Level;
				UpdateMark(m_WeaponStat);
			}

			SetWeaponLevelUp();
			SetWeaponDesc();
			RefreshTalent();
			g_PSCharacterMgr->InitCharacterStat();
		}
	}
}

void UPSWidget_WeaponInfo::OnFinishHideAni()
{
	Super::OnFinishHideAni();

	if(Overlay_InfoDesc)
	{
		if(Overlay_InfoDesc->GetChildrenCount() > 0)
		{
			Overlay_InfoDesc->RemoveChildAt(0);
		}
	}
}

FString UPSWidget_WeaponInfo::FormatFloatWithCondition(float Value)
{
	// 정수 여부를 넓은 범위로 확인 (더 넓은 범위 사용)
	if (FMath::IsNearlyEqual(Value, FMath::RoundToFloat(Value), 0.01f)) // 0.01f로 비교
		{
		return FString::Printf(TEXT("%d"), static_cast<int32>(FMath::RoundToFloat(Value)));
		}
	else
	{
		return FString::Printf(TEXT("%.1f"), Value);
	}
}

FString UPSWidget_WeaponInfo::ReplacePlaceholders(const FString& Script, const FString& Value, const FString& Percent)
{
	FString Result = Script;
	Result = Result.Replace(TEXT("{Value}"), *FString::Printf(TEXT("<Y>%s</>"), *Value));
	Result = Result.Replace(TEXT("{TriggerValue}"), *FString::Printf(TEXT("<Y>%s</>"), *Percent));
	return Result;
}

void UPSWidget_WeaponInfo::UpdateMark(FWeaponStat* WeaponStat)
{
	int32 Level = WeaponStat->Level;

	bool bCanLevelUp = false;

	if (Level > 0)
	{
		if (PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(Level)))
		{
			m_eRateType = WeaponStat->m_eRateType;
			if (pRecord->m_mapPiece.Contains(m_eRateType))
			{
				int32 MaxPiece = pRecord->m_mapPiece[m_eRateType];
				int32 MaxGold = pRecord->m_mapGold[m_eRateType];
                
				bCanLevelUp = (m_Piece >= MaxPiece) && (g_PSCharacterMgr->GetGold() >= MaxGold) && WeaponStat->m_IsLock;
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

void UPSWidget_WeaponInfo::UpdateProgressBar()
{
	if (ProgressBar_Piece && m_WeaponStat)
	{
		if (PSWeaponPieceRecord* pRecord = g_WeaponPieceRecordMgr->FindRecord(FString::FromInt(m_WeaponStat->Level)))
		{
			if (pRecord->m_mapPiece.Contains(m_WeaponStat->m_eRateType))
			{
				int32 maxPiece = m_MaxPiece = pRecord->m_mapPiece[m_eRateType];
				float progress = (maxPiece > 0) ? static_cast<float>(m_Piece) / static_cast<float>(maxPiece) : 0.0f;

				ProgressBar_Piece->SetPercent(FMath::Clamp(progress, 0.0f, 1.0f));

				FLinearColor fillColor = (progress >= 1.0f) 
										 ? FLinearColor(0.0f, 0.796f, 0.184f, 1.0f)
										 : FLinearColor(1.0f, 0.718f, 0.0f, 1.0f);
				ProgressBar_Piece->SetFillColorAndOpacity(fillColor);
			}
		}
	}
}




