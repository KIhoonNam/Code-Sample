// Fill out your copyright notice in the Description page of Project Settings.


#include "PSScrollWidget_AcquiredProperty.h"

#include "PSWidgetSlot_AcquiredProperty.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSScrollWidget_AcquiredProperty::InitData()
{
	Super::InitData();

	if (ScrollBox_Base)
	{
		ScrollBox_Base->ClearChildren();
	}
}

void UPSScrollWidget_AcquiredProperty::ReleaseData()
{
	Super::ReleaseData();
}

void UPSScrollWidget_AcquiredProperty::LinkEvent()
{
	Super::LinkEvent();

	if (g_PSPropertiesMgrValid)
	{
		g_PSPropertiesMgr->OnPropertySelected.AddDynamic(this, &UPSScrollWidget_AcquiredProperty::OnPropertySelected);
	}
}

void UPSScrollWidget_AcquiredProperty::UnLinkEvent()
{
	if (g_PSPropertiesMgrValid)
	{
		g_PSPropertiesMgr->OnPropertySelected.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSScrollWidget_AcquiredProperty::OnPropertySelected()
{
	if (NoPropertyText)
	{
		NoPropertyText->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (ScrollBox_Base)
	{
		if (g_PSPropertiesMgrValid)
		{
			TArray<FPropertiesData*> arrPropertyData = g_PSPropertiesMgr->m_arrPropertyData;
			if (arrPropertyData.Num() >= ScrollBox_Base->GetChildrenCount())
			{
				if (UClass* slotClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "S_AcquiredProperty_BP"))
				{
					if (UPSWidgetSlot_AcquiredProperty* pSlot = CreateWidget<UPSWidgetSlot_AcquiredProperty>(this, slotClass))
					{
						ScrollBox_Base->AddChild(pSlot);

						FMargin margin(0.f, 10.f);
						pSlot->SetPadding(margin);
						
						if (arrPropertyData.Last())
						{
							FString ScriptWithValue = arrPropertyData.Last()->m_strScript;
							int32 RoundedValue = FMath::RoundToInt(arrPropertyData.Last()->m_fValue);
							int32 RoundedTriggerValue = FMath::RoundToInt(arrPropertyData.Last()->m_fTriggerValue);

							FString ModifiedScript = ReplacePlaceholders(ScriptWithValue,
							FString::FromInt(RoundedValue),
							FString::FromInt(RoundedTriggerValue));

							pSlot->PropertyDesc->SetText(FText::FromString(ModifiedScript));
							pSlot->PropertyIcon->SetBrushFromSpriteName(arrPropertyData.Last()->m_strSprite);
							pSlot->BackgroundImage->SetBrushFromSpriteName(arrPropertyData.Last()->m_strSPRBack);
						}
					}
				}
			}
		}
	}
}

FString UPSScrollWidget_AcquiredProperty::ReplacePlaceholders(const FString& Script, const FString& Value, const FString& Percent)
{
	FString Result = Script;
	Result = Result.Replace(TEXT("{Value}"), *FString::Printf(TEXT("<Y>%s</>"), *Value));
	Result = Result.Replace(TEXT("{TriggerValue}"), *FString::Printf(TEXT("<Y>%s</>"), *Percent));
	return Result;
}
