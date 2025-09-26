// Fill out your copyright notice in the Description page of Project Settings.

#include "PSScrollWidget_Stage.h"

#include "PSWidgetSlot_Stage.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "ProtoSurvivor/DataTable/DataManager/PSClearBonusRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSClearBonusRecord.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"

void UPSScrollWidget_Stage::InitData()
{
	Super::InitData();

	if (g_DataBaseMgrValid)
	{
		if (g_DataBaseMgr->GetIsLoad())
		{
			InitScrollBox();
		}
	}
}

void UPSScrollWidget_Stage::ReleaseData()
{
	Super::ReleaseData();
}

void UPSScrollWidget_Stage::LinkEvent()
{
	Super::LinkEvent();

	if (g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this, &UPSScrollWidget_Stage::InitScrollBox);
	}
	
}

void UPSScrollWidget_Stage::UnLinkEvent()
{
	if (g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}



void UPSScrollWidget_Stage::ScrollToSlot(int32 SlotIndex)
{
	if (ScrollBox_Base)
	{
		if(ScrollBox_Base->GetSlots().IsValidIndex(SlotIndex))
		{
			if(UPanelSlot* pPanelSlot = ScrollBox_Base->GetSlots()[SlotIndex])
			{
				ScrollBox_Base->ScrollWidgetIntoView(pPanelSlot->Content, true, EDescendantScrollDestination::Center);
				m_CurrentIndex = SlotIndex+1;
			}
		}
	}
}

void UPSScrollWidget_Stage::InitScrollBox()
{
	if (ScrollBox_Base)
	{
		if (g_StageMgrValid)
		{
			if (UClass* slotClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "S_Stage_BP"))
			{
				TArray<PSStageRecord*> stageRecords = g_StageMgr->m_arrStageRecord;
				for (int32 stageIndex = 0; stageIndex < stageRecords.Num(); stageIndex++)
				{
					if (UPSWidgetSlot_Stage* pSlot = CreateWidget<UPSWidgetSlot_Stage>(this, slotClass))
					{
						ScrollBox_Base->AddChild(pSlot);

						FMargin margin(80.f, 0.f);
						pSlot->SetPadding(margin);
						
						pSlot->SlotButton_Image_Icon->SetBrushFromSpriteName(stageRecords[stageIndex]->m_strStageSpriteName);
						if (g_StageMgr->GetMaxStageNumber() <= stageIndex)
						{
							pSlot->SlotButton_Image_Icon->SetColorAndOpacity(FLinearColor::Black);
						}
						pSlot->SetClickEvent(this);
					}
				}
			}

			ScrollToSlot(g_StageMgr->GetMaxStageNumber() - 1);
		}
	}
}
