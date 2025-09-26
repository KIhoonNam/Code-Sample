// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_Properties.h"

#include "CoreLibrary/GameGlobal/CSGameGlobal.h"
#include "ProtoSurvivor/Character/PSMonsterBase.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSPropertiesRecord.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "ProtoSurvivor/Weapon/WeaponType.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "Components/RichTextBlock.h"

void UPSWidgetSlot_Properties::InitData()
{
	Super::InitData();
}

void UPSWidgetSlot_Properties::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidgetSlot_Properties::LinkEvent()
{
	Super::LinkEvent();
}

void UPSWidgetSlot_Properties::UnLinkEvent()
{
	Super::UnLinkEvent();
}

FString UPSWidgetSlot_Properties::ReplacePlaceholders(const FString& Script, const FString& Value, const FString& Percent)
{
    FString Result = Script;
    Result = Result.Replace(TEXT("{Value}"), *FString::Printf(TEXT("<Y>%s</>"), *Value));
    Result = Result.Replace(TEXT("{TriggerValue}"), *FString::Printf(TEXT("<Y>%s</>"), *Percent));
    return Result;
}

void UPSWidgetSlot_Properties::SetSlotPropertiesState(TArray<PSPropertiesRecord*>& AvailableRecords)
{    
    if (RichTextBlock_Text_Title && SlotButton_Image_Icon && AvailableRecords.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableRecords.Num() - 1);
        PSPropertiesRecord* pRecord = AvailableRecords[RandomIndex];

        if (pRecord)
        {
            // 버프 확률 처리
            ApplyRandomChanceForBuff(pRecord);

            m_PropertiesData = pRecord->m_Properties;

            if (m_PropertiesData && RichTextBlock_Text_Title && SlotButton_Image_Icon)
            {
                
                FString ScriptWithValue = ReplacePlaceholders(
                    m_PropertiesData->m_strScript,
                    FString::FromInt(m_PropertiesData->m_fValue),
                    FString::FromInt(m_PropertiesData->m_fTriggerValue)
                    );
                
                    RichTextBlock_Text_Title->SetText(FText::FromString(ScriptWithValue));

                SlotButton_Image_Icon->SetBrushFromSpriteName(m_PropertiesData->m_strSprite);
            }

            AvailableRecords.RemoveAt(RandomIndex);

            if(SlotImage_CoolDown)
            {
                SlotImage_CoolDown->SetBrushFromSpriteName(m_PropertiesData->m_strSPRBack);
            }
        }
    }
}


void UPSWidgetSlot_Properties::SelectProperties()
{
    if (!m_PropertiesData || !g_PSBackPackMgrValid) return;

    TArray<FString> BackpackTypes;
    for (const TPair<int32, FWeaponStat*>& Pair : g_PSBackPackMgr->m_MapBackPackWeapon)
    {
        if (Pair.Value && !Pair.Value->m_strTid.IsEmpty())
        {
            BackpackTypes.AddUnique(Pair.Value->m_strTid);
        }
    }

    if (m_PropertiesData->m_strType == TEXT("Default"))
    {
        HandleDefaultBuff(m_PropertiesData->m_eBuffType);
        if (g_PSPropertiesMgrValid)
        {
            g_PSPropertiesMgr->m_arrPropertyData.Emplace(m_PropertiesData);
            if (g_PSPropertiesMgr->OnPropertySelected.IsBound())
            {
                g_PSPropertiesMgr->OnPropertySelected.Broadcast();
            }
        }
        return;
    }

    if (BackpackTypes.Contains(m_PropertiesData->m_strType))
    {
        HandleBackPackBuff(m_PropertiesData->m_eBuffType);
        if (g_PSPropertiesMgrValid)
        {
            g_PSPropertiesMgr->m_arrPropertyData.Emplace(m_PropertiesData);
            if (g_PSPropertiesMgr->OnPropertySelected.IsBound())
            {
                g_PSPropertiesMgr->OnPropertySelected.Broadcast();
            }
        }
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("%s 버프를 찾을 수 없습니다."), *m_PropertiesData->m_strType);
}


void UPSWidgetSlot_Properties::HandleDefaultBuff(nBuffType::en BuffType)
{
    if (!g_PSPropertiesMgrValid || !m_PropertiesData) return;

    if (BuffType == nBuffType::Critical)
    {
        float CriticalBoostValue = GetCriticalBoostValue();
        g_PSPropertiesMgr->ApplyGlobalBuff(BuffType, m_PropertiesData->m_strTid, CriticalBoostValue);
    }
    else
    {
        g_PSPropertiesMgr->ApplyGlobalBuff(BuffType, m_PropertiesData->m_strTid);
    }
}

void UPSWidgetSlot_Properties::HandleBackPackBuff(nBuffType::en BuffType)
{
    if (!g_PSBackPackMgrValid || !m_PropertiesData) return;

    const FString& TargetType = m_PropertiesData->m_strType;

    for (const TPair<int32, FWeaponStat*>& Pair : g_PSBackPackMgr->m_MapBackPackWeapon)
    {
        if (Pair.Value && Pair.Value->m_strTid == TargetType)
        {
            g_PSPropertiesMgr->ApplyWeaponBuff(BuffType, Pair.Value, m_PropertiesData->m_strTid);
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("%s 무기를 찾을 수 없습니다."), *TargetType);
}


void UPSWidgetSlot_Properties::ApplyRandomChanceForBuff(PSPropertiesRecord* pRecord)
{
    if (pRecord->m_strTid == TEXT("AllCriticalBoost"))
    {
        int32 RandomChance = FMath::RandRange(1, 100);
        if (RandomChance <= 70)
        {
            pRecord->m_Properties->m_strScript = TEXT("모든 무기 치명타 확률 + {Value}%");
        }
        else if (RandomChance <= 90)
        {
            pRecord->m_Properties->m_strScript = TEXT("모든 무기 치명타 확률 + {Value}%");
        }
        else
        {
            pRecord->m_Properties->m_strScript = TEXT("모든 무기 치명타 확률 + {Value}%");
        }
    }
}

float UPSWidgetSlot_Properties::GetCriticalBoostValue() const
{
    if (!m_PropertiesData) return 1.0f; // 기본값

    if (m_PropertiesData->m_strScript.Contains(TEXT("+ 1%")))
        return 1.0f;
    else if (m_PropertiesData->m_strScript.Contains(TEXT("+ 3%")))
        return 3.0f;
    else if (m_PropertiesData->m_strScript.Contains(TEXT("+ 5%")))
        return 5.0f;

    return 1.0f; // 기본값
}





