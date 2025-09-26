// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_Properties.h"

#include "PSWidgetSlot_CoolDownItem.h"
#include "PSWidgetSlot_Properties.h"
#include "Components/RichTextBlock.h"
#include "CoreLibrary/GameGlobal/CSGameGlobal.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/DataTable/DataManager/PSPropertiesRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSPropertiesRecord.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "ProtoSurvivor/Manager/PSAdMobMgr.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSVerticalBox.h"

void UPSWidget_Properties::InitData()
{
	Super::InitData();
	
	if(Button_ADRefresh)
	{
		Button_ADRefresh->SetIsEnabled(true);
	}

	if (g_PSCharacterMgrValid)
	{
		if (g_PSCharacterMgr->GetPurchasedAdRemove())
		{
			SetAdRemove();
		}
	}
	
	RefreshNum = 3;
	AllNum = 3;
}

void UPSWidget_Properties::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_Properties::LinkEvent()
{
	Super::LinkEvent();

	if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
	{
		PSCharacter->OnPlayerSelectPropertiesed.AddDynamic(this,&UPSWidget_Properties::InitProperties);
	}
	
	BIND_BUTTON_EVENT(Button_ADRefresh, &UPSWidget_Properties::OnClicked_ADRefresh);
	BIND_BUTTON_EVENT(Button_ADAll, &UPSWidget_Properties::OnClicked_ADAll);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.AddUObject(this, &UPSWidget_Properties::SetAdRemove);
	}
}

void UPSWidget_Properties::UnLinkEvent()
{

	if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
	{
		PSCharacter->OnPlayerSelectPropertiesed.RemoveDynamic(this, &UPSWidget_Properties::InitProperties);
	}
	UNBIND_BUTTON_EVENT_ALL(Button_ADRefresh);
	UNBIND_BUTTON_EVENT_ALL(Button_ADAll);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidget_Properties::InitProperties()
{
	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->AddPauseReason(GetWorld(), nPauseType::Properties);
	}
	Show();
	if(Properties_Animation)
	PlayAnimation(Properties_Animation);
	Refresh();
}

void UPSWidget_Properties::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);

	if(UPSWidgetSlot_Properties* pSlot = Cast<UPSWidgetSlot_Properties>(_pChildWidget))
	{
		pSlot->SelectProperties();
		
		if (g_PSPauseMgrValid)
		{
			g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::Properties);
		}
		
		PSPropertiesRecord* pSlotRecord = g_PropertiesRecordMgr->FindRecord(pSlot->m_PropertiesData->m_strTid);
		if (pSlotRecord && !SelectedRecords.Contains(pSlotRecord))
		{
			if (!pSlotRecord->m_Properties->m_bOverlap)
			{
				SelectedRecords.Add(pSlotRecord);
			}
		}
		
		ReSetArmorHP();
		
		Hide();
	}
}

void UPSWidget_Properties::ReSetArmorHP()
{
	float TotalHP = 0.0f; // CoolDownSlot들의 HP 합산 값 초기화

	for (UPSWidgetSlot_CoolDownItem* CoolDownSlot : g_PSBackPackMgr->ActiveCoolDownSlots)
	{
		if (CoolDownSlot && CoolDownSlot->m_pWeaponStat)
		{
			nTargetType::en TargetType = CoolDownSlot->m_pWeaponStat->m_eTargetType;
			if (TargetType == nTargetType::HP || TargetType == nTargetType::Armor)
			{
				TotalHP += CoolDownSlot->m_pWeaponStat->GetTotalHP();
			}
		}
	}

	// 합산한 TotalHP와 ArmorHP를 비교하여 처리
	if (APSCharacterBase* character = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
	{
		if (TotalHP != character->GetArmorHP())
		{
			float HPPercent = character->GetCurrentHP() / character->GetMaxHP();
			character->SetArmorHP(0); // ArmorHP 초기화
			character->SetArmorHP(character->GetArmorHP() + TotalHP); // TotalHP 추가
			character->SetCurrentHP_Heal(character->GetMaxHP() * HPPercent - character->GetCurrentHP());
		}
	}
}

void UPSWidget_Properties::OnClicked_ADRefresh()
{
	UE_LOG(LogTemp,Log,TEXT("광고 새로고침"));

	RefreshNum -= 0;

	RichTextblock_RefreshNum->SetText(FText::FromString(FString::Printf(TEXT("남은 시도 횟수 <G>%d/3</>"), RefreshNum)));

	if(RefreshNum <= 0)
	{
		if(VerticalBox_All->GetVisibility() == ESlateVisibility::Collapsed)
		{
			VerticalBox_Refresh->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			VerticalBox_Refresh->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (g_PSAdMobMgrValid)
	{
		g_PSAdMobMgr->ShowRewardAd_PropertyRefresh(0, "PropertyRefresh", this);
	}
}

void UPSWidget_Properties::OnClicked_ADAll()
{
	UE_LOG(LogTemp,Log,TEXT("광고 모두 선택"));

	AllNum -= 1;
	
	RichTextblock_AllNum->SetText(FText::FromString(FString::Printf(TEXT("남은 시도 횟수 <G>%d/3</>"), AllNum)));

	if(AllNum <= 0)
	{
		if(VerticalBox_Refresh->GetVisibility() == ESlateVisibility::Collapsed)
		{
			VerticalBox_All->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			VerticalBox_All->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (g_PSAdMobMgrValid)
	{
		g_PSAdMobMgr->ShowRewardAd_PropertyAll(0, "PropertyAll", this);
	}

	ReSetArmorHP();
}

void UPSWidget_Properties::Refresh()
{
    if (ScrollBox_Properties)
    {
        UCSScrollBox* pScrollBox = ScrollBox_Properties->GetScrollBox();
        if (pScrollBox)
        {
            // 기존 슬롯 숨기기
            for (int idx = 0; idx < pScrollBox->GetChildrenCount(); ++idx)
            {  
                if (UPSWidgetSlot_Properties* pSlot = Cast<UPSWidgetSlot_Properties>(pScrollBox->GetChildAt(idx)))
                {
                    pSlot->Hide();
                }
            }

            if (g_PSBackPackMgrValid)
            {
                int32 WeaponNum = g_PSBackPackMgr->m_MapBackPackWeapon.Num();
                TArray<int32> arrEquipWeaponKeys;
                g_PSBackPackMgr->m_MapBackPackWeapon.GetKeys(arrEquipWeaponKeys);

                TArray<PSPropertiesRecord*> arrProperties;
                g_PropertiesRecordMgr->GetAllRecord(arrProperties);
                TArray<PSPropertiesRecord*> MatchedProperties;
                TArray<PSPropertiesRecord*> DefaultProperties;

            	bool bHasUpgradableWeapon = CheckUpgradableWeapon();
            	
                for (PSPropertiesRecord* Record : arrProperties)
                {
                    if (Record && !SelectedRecords.Contains(Record))
                    {
                        if (Record->m_Properties->m_strType == TEXT("Default"))
                        {
                        	if (Record->m_Properties->m_eBuffType == nBuffType::UpGrade && !bHasUpgradableWeapon)
                        	{
                        		continue;
                        	}
                            DefaultProperties.Add(Record);
                        }
                        else if (WeaponNum > 0)
                        {
                            for (int32 Key : arrEquipWeaponKeys)
                            {
                                if (FWeaponStat* pWeaponStat = g_PSBackPackMgr->m_MapBackPackWeapon[Key])
                                {
                                    if (pWeaponStat->m_strTid == Record->m_Properties->m_strType)
                                    {
                                    	if(pWeaponStat->Level >= Record->m_Properties->m_Level)
                                    	{
                                    		MatchedProperties.Add(Record);
                                    	}
                                    }
                                }
                            }
                        }
                    }
                }

                TArray<PSPropertiesRecord*> CombinedProperties = DefaultProperties;
                CombinedProperties.Append(MatchedProperties);

            	CombinedProperties.Sort([](const PSPropertiesRecord& A, const PSPropertiesRecord& B) {
					return FMath::RandBool(); // 랜덤 재정렬
				});
            	
                for (int idx = 0; idx < 3; ++idx)
                {
                    UPSWidgetSlot_Properties* pSlot = nullptr;
                    bool bExistSlot = pScrollBox->GetChildrenCount() > idx;

                    if (bExistSlot)
                    {
                        pSlot = Cast<UPSWidgetSlot_Properties>(pScrollBox->GetChildAt(idx));
                        if (pSlot)
                        {
                            pSlot->SetSlotIdx(idx);
                            pSlot->SetSlotPropertiesState(CombinedProperties);

                            UE_LOG(LogTemp, Warning, TEXT("슬롯 [%d]: TID = %s"), idx, *pSlot->m_PropertiesData->m_strScript);
                        }
                    }
                    else
                    {
                        if (UPSWidgetSlot_Properties* pChildSlot = Cast<UPSWidgetSlot_Properties>(CSUIUtils::LoadWidget(TEXT("S_Properties_BP"), GetWorld())))
                        {
                            pChildSlot->SetSlotIdx(idx);
                            ScrollBox_Properties->AddScrollChild(pChildSlot, this);
                            pChildSlot->SetSlotPropertiesState(CombinedProperties);

                            UE_LOG(LogTemp, Warning, TEXT("생성된 슬롯 [%d]: TID = %s"), idx, *pChildSlot->m_PropertiesData->m_strScript);

                            pSlot = pChildSlot;
                        }
                    }
                    if (pSlot)
                    {
                        pSlot->Show();
                    }
                }

                ScrollBox_Properties->AddSelectedChild(0);
            }
        }
    }
}

bool UPSWidget_Properties::CheckUpgradableWeapon() const
{
    if (!g_PSBackPackMgrValid || g_PSBackPackMgr->ActiveCoolDownSlots.Num() == 0)
    {
        return false;
    }

    for (UPSWidgetSlot_CoolDownItem* CoolDownSlot : g_PSBackPackMgr->ActiveCoolDownSlots)
    {
        if (CoolDownSlot && CoolDownSlot->m_pWeaponStat)
        {
	        int32 CurrentLevel = CoolDownSlot->m_pWeaponStat->UpgradeLevel;
        	int32 MaxUpgradeLevel = CoolDownSlot->m_pWeaponStat->MaxUpgradeLevel;

        	FString BaseSpriteName = CoolDownSlot->m_pWeaponStat->m_strTid;
        	FString CurrentSpriteName = FString::Printf(TEXT("%s_%d_Sprite"), *BaseSpriteName, CurrentLevel);

        	if (!CSUIUtils::LoadSprite(CurrentSpriteName))
        	{
        		UE_LOG(LogTemp, Error, TEXT("Failed to load sprite for CurrentLevel: %d"), CurrentLevel);
        		continue;
        	}

        	if (CurrentLevel < MaxUpgradeLevel)
        	{
        		FString NextSpriteName = FString::Printf(TEXT("%s_%d_Sprite"), *BaseSpriteName, CurrentLevel + 1);

        		if (CSUIUtils::LoadSprite(NextSpriteName))
        		{
        			return true;
        		}
        	}
        }
    }
    return false;
}





void UPSWidget_Properties::GetAllProperties()
{
	if (ScrollBox_Properties)
	{
		if (UCSScrollBox* pScrollBox = ScrollBox_Properties->GetScrollBox())
		{
			for (auto& propertySlot: pScrollBox->GetWidgetChildren())
			{
				if (UPSWidgetSlot_Properties* pSlot = Cast<UPSWidgetSlot_Properties>(propertySlot))
				{
					pSlot->SelectProperties();

					PSPropertiesRecord* pSlotRecord = g_PropertiesRecordMgr->FindRecord(pSlot->m_PropertiesData->m_strTid);
					if (pSlotRecord && !SelectedRecords.Contains(pSlotRecord))
					{
						if (!pSlotRecord->m_Properties->m_bOverlap)
						{
							SelectedRecords.Add(pSlotRecord);
						}
					}
				}
			}

			if (g_PSPauseMgrValid)
			{
				g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::Properties);
			}
			
			Hide();
		}
	}
}

void UPSWidget_Properties::SetAdRemove()
{
	if (AdIconImage_All)
	{
		AdIconImage_All->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (AdIconImage_Refresh)
	{
		AdIconImage_Refresh->SetVisibility(ESlateVisibility::Collapsed);
	}
}



