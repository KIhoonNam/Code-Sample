// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_TopMenu.h"

#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "Components/Overlay.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"

void UPSWidgetSlot_TopMenu::InitData()
{
	Super::InitData();

	if (!strGoodsType.IsEmpty())
	{
		m_eGoodsType = nGoodsType::GetEnum(strGoodsType, false);
	}

	if (m_eGoodsType != nGoodsType::Energy)
	{
		Overlay_Timer->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (g_PSCharacterMgrValid)
	{
		float EnergyAccumulator = g_PSCharacterMgr->GetEnergyAccumulator();
		int32 Minutes = static_cast<int32>(EnergyAccumulator) / 60;
		int32 Seconds = static_cast<int32>(EnergyAccumulator) % 60;

		FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		if (TextBlock_EnergyTime)
		{
			TextBlock_EnergyTime->SetText(FText::FromString(TimeText));
		}
	}

	ChangeGoods();
}


void UPSWidgetSlot_TopMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Overlay_Timer->GetVisibility() == ESlateVisibility::Visible && g_PSCharacterMgrValid)
	{
		float EnergyAccumulator = g_PSCharacterMgr->GetEnergyAccumulator();

		int32 Minutes = static_cast<int32>(EnergyAccumulator) / 60;
		int32 Seconds = static_cast<int32>(EnergyAccumulator) % 60;

		if (TextBlock_EnergyTime)
		{
			FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
			TextBlock_EnergyTime->SetText(FText::FromString(TimeText));
		}
	}
}



void UPSWidgetSlot_TopMenu::LinkEvent()
{
	Super::LinkEvent();

	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->RefreshGoods.AddUObject(this,&UPSWidgetSlot_TopMenu::ChangeGoods);
	}
}

void UPSWidgetSlot_TopMenu::UnLinkEvent()
{
	if(g_PSCharacterMgrValid)
	{
		g_PSCharacterMgr->RefreshGoods.RemoveAll(this);
	}
	Super::UnLinkEvent();
}

void UPSWidgetSlot_TopMenu::ChangeGoods()
{
	if(g_PSCharacterMgrValid)
	{
		if(SlotButton_Text_Title)
		{
			switch (m_eGoodsType)
			{
			case nGoodsType::Dia:
				{
					SlotButton_Text_Title->SetText(FText::FromString(g_PSCharacterMgr->GetstrDia()));
				}break;
			case nGoodsType::Energy:
				{
					FString strEnergy = g_PSCharacterMgr->GetstrEnergy();
					
					Overlay_Timer->SetVisibility(LogIfEnergyIsLessThanMax(strEnergy) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
					
					SlotButton_Text_Title->SetText(FText::FromString(strEnergy));
				} break;
			case nGoodsType::Gold:
				{
					SlotButton_Text_Title->SetText(FText::FromString(g_PSCharacterMgr->GetstrGold()));
				}break;
			default:
				break;
			}
		}
	}
}

bool UPSWidgetSlot_TopMenu::LogIfEnergyIsLessThanMax(const FString& strEnergy)
{
	FString strLeftEnergy, strRightEnergy;
	strEnergy.Split(TEXT("/"), &strLeftEnergy, &strRightEnergy);

	float currentEnergy = FCString::Atof(*strLeftEnergy);
	float maxEnergy = FCString::Atof(*strRightEnergy);

	if (currentEnergy < maxEnergy)
	{
		return true;
	}

	return false;
}

void UPSWidgetSlot_TopMenu::UpdateEnergyTimeDisplay()
{
	if (TextBlock_EnergyTime)
	{
		int32 Minutes = RemainingSeconds / 60;
		int32 Seconds = RemainingSeconds % 60;

		FString TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TextBlock_EnergyTime->SetText(FText::FromString(TimeText));
	}
}





