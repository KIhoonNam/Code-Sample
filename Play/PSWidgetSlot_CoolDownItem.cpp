#include "PSWidgetSlot_CoolDownItem.h"

#include "NiagaraSystemWidget.h"
#include "PaperSprite.h"
#include "CoreLibrary/GameGlobal/CSGameGlobal.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponDetailRecordMgr.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidgetSlot_CoolDownItem::SetItemImage(UPaperSprite* NewItemImage)
{
    if (IsValid(NewItemImage))
    {
        itemImage = NewItemImage;
        SlotButton_Image_Icon->SetBrushFromSpriteName(itemImage->GetName());
    }

    if(m_pWeaponStat)
    {
        if(TextBlock_Cost)
        {
            TextBlock_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d"), int(m_pWeaponStat->m_fCost))));
            DynamicMaterial = SlotImage_CoolDown->GetDynamicMaterial();
            UE_LOG(LogTemp,Warning,TEXT("쿨타임생성: %s"), *m_pWeaponStat->m_strName);
        }

        if(SlotButton_Image_Border->IsVisible())
        {
            SlotButton_Image_Border->SetBrushFromSpriteName(m_pWeaponStat->m_strRateSpriteName);
        }
    }

    CoolDownStartTime = UGameplayStatics::GetTimeSeconds(this);
    CoolDownElapsedTime = 0.0f;

}

void UPSWidgetSlot_CoolDownItem::InitData()
{
    Super::InitData();
    
    if (IsValid(itemImage))
    {
        SlotButton_Image_Icon->SetBrushFromSpriteName(itemImage->GetName());
    }
}

void UPSWidgetSlot_CoolDownItem::AdjustCooldownStart()
{
    if (!g_PSBackPackMgr || !m_pWeaponStat)
        return;

    int32 SlotIndex = 0;
    int32 SameTIDCount = 0;

    for (UPSWidgetSlot_CoolDownItem* ActiveSlot : g_PSBackPackMgr->ActiveCoolDownSlots)
    {
        if (ActiveSlot && ActiveSlot->m_pWeaponStat->m_strTid == m_pWeaponStat->m_strTid)
        {
            if (ActiveSlot == this)
            {
                SlotIndex = SameTIDCount;
            }
            SameTIDCount++;
        }
    }

    if (SameTIDCount > 0)
    {
        float CooldownStartOffsetPercent = static_cast<float>(SlotIndex) / SameTIDCount;
        float CooldownOffsetTime = CooldownStartOffsetPercent * m_pWeaponStat->GetTotalCoolTime();
        CoolDownStartTime = UGameplayStatics::GetTimeSeconds(this) - CooldownOffsetTime;
    }
}

void UPSWidgetSlot_CoolDownItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (DynamicMaterial && m_pWeaponStat)
    {
        float CurrentTime = UGameplayStatics::GetTimeSeconds(this);

        if (!bIsCooldownAdjusted)
        {
            AdjustCooldownStart();
            bIsCooldownAdjusted = true;
        }

        float CurrentCombatSpeed = g_PSCombatMgr->GetCombatSpeed();

        if (FMath::Abs(CurrentCombatSpeed - LastCombatSpeed) > KINDA_SMALL_NUMBER)
        {
            float weaponCooldownOld = m_pWeaponStat->GetTotalCoolTime() / LastCombatSpeed;
            float ElapsedRatio = FMath::Clamp((CurrentTime - CoolDownStartTime) / weaponCooldownOld, 0.0f, 1.0f);

            float weaponCooldownNew = m_pWeaponStat->GetTotalCoolTime() / CurrentCombatSpeed;
            CoolDownStartTime = CurrentTime - (ElapsedRatio * weaponCooldownNew);

            LastCombatSpeed = CurrentCombatSpeed;
        }

        CoolDownElapsedTime = CurrentTime - CoolDownStartTime;

        float CooldownMultiplier = g_PSCombatMgr->IsCooldownReductionActive() ? g_PSCombatMgr->GetCooldownReductionMultiplier() : 1.0f;
        float weaponCooldown = (m_pWeaponStat->GetTotalCoolTime() * CooldownMultiplier) / CurrentCombatSpeed;
        float Percent = FMath::Clamp((CoolDownElapsedTime * CooldownMultiplier) / weaponCooldown, 0.001f, 1.0f);
        DynamicMaterial->SetScalarParameterValue("percent", Percent);

        if (Percent >= 1.0f)
        {
            if (m_pWeaponStat->m_eTargetType == nTargetType::Armor ||
                m_pWeaponStat->m_eTargetType == nTargetType::HP)
            {
                g_PSCombatMgr->SpawnArmor(m_pWeaponStat);
            }
            else
            {
                g_PSCombatMgr->SpawnWeapon(m_pWeaponStat);
            }

            CoolDownElapsedTime = 0.0f;
            CoolDownStartTime = CurrentTime;
            DynamicMaterial->SetScalarParameterValue("percent", 0.001f);
        }
    }
}


void UPSWidgetSlot_CoolDownItem::LinkEvent()
{
    Super::LinkEvent();
}

void UPSWidgetSlot_CoolDownItem::UnLinkEvent()
{
    Super::UnLinkEvent();
}

void UPSWidgetSlot_CoolDownItem::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
    Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

}

void UPSWidgetSlot_CoolDownItem::Hide(float fDelatTime, float fFadeTime)
{
    Super::Hide(fDelatTime, fFadeTime);
}

bool UPSWidgetSlot_CoolDownItem::WeaponUpgrade()
{
    float UpgradeValue = g_CSDefineRecordMgr->GetValue_Number("GV_WEAPON_UPGRADE_VALUE");

    if (m_pWeaponStat)
    {
        FString strSpriteUpgrade = m_pWeaponStat->m_strSpriteName;
        UpgradeImage(strSpriteUpgrade);

        if (!CSUIUtils::LoadSprite(strSpriteUpgrade))
        {
            UE_LOG(LogTemp, Warning, TEXT("강화 실패: 유효하지 않은 스프라이트 이름 %s"), *strSpriteUpgrade);
            return false;
        }
        if(m_pWeaponStat)
        {
            m_pWeaponStat->UpgradeLevel = Number;
        }
        switch (m_pWeaponStat->m_eTargetType)
        {
        case nTargetType::Near:
            m_pWeaponStat->DamageWeaponUpgrade(UpgradeValue);
            break;
            
        case nTargetType::Random:
            m_pWeaponStat->DamageWeaponUpgrade(UpgradeValue);
            break;

        case nTargetType::Armor:
            m_pWeaponStat->m_fArmor *= UpgradeValue;
            m_pWeaponStat->m_fHP *= UpgradeValue;
            break;

        case nTargetType::HP:
            m_pWeaponStat->m_fHP *= UpgradeValue;
            m_pWeaponStat->m_fHeal *= UpgradeValue;
            break;

        default:
            UE_LOG(LogTemp, Warning, TEXT("알 수 없는 대상 유형: 강화하지 않음"));
            break;
        }
        m_pWeaponStat->m_strSpriteName = strSpriteUpgrade;
        SetItemImage(m_pWeaponStat);

        if(Niagara_Upgrade)
        {
            Niagara_Upgrade->ActivateSystem(true);
        }

        if (SlotButton_Image_Icon)
        {
            SlotButton_Image_Icon->SetBrushFromSpriteName(m_pWeaponStat->m_strSpriteName);
            return true;
        }
    }
    return false;
}

void UPSWidgetSlot_CoolDownItem::SetItemImage(FWeaponStat* WeaponStat)
{
	if (UPaperSprite* pTexture = CSUIUtils::LoadSprite(WeaponStat->m_strSpriteName))
	{
		if (IsValid(pTexture))
		{
			itemImage = pTexture;
		}
	}
}

void UPSWidgetSlot_CoolDownItem::UpgradeImage(FString& InOutSpriteName)
{
	FString BaseName;
	FString NumberPart;
  

	if (InOutSpriteName.RemoveFromEnd("_Sprite") &&
		InOutSpriteName.Split("_", &BaseName, &NumberPart))
	{
		if (LexTryParseString(Number, *NumberPart))
		{
			Number++;
			InOutSpriteName = FString::Printf(TEXT("%s_%d_Sprite"), *BaseName, Number);
			UE_LOG(LogTemp, Log, TEXT("업그레이드된 스프라이트 이름: %s"), *InOutSpriteName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("숫자 파싱 실패: %s"), *NumberPart);
			InOutSpriteName.Empty();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprite 이름 형식이 맞지 않음: %s"), *InOutSpriteName);
		InOutSpriteName.Empty();
	}
}