// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_WeaponInfo.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"


void UPSWidgetSlot_WeaponInfo::SynchronizeProperties()
{
	Super::SynchronizeProperties();


	if(Image_Value)
	{
		Image_Value->SetBrush(m_Image);
	}

	if(Text_Desc)
	{
		Text_Desc->SetText(FText::FromString(m_strTextTitle.ToString()));
	}
}

void UPSWidgetSlot_WeaponInfo::SetValueText(const FString& _strValue)
{
	if(Text_Value)
	{
		Text_Value->SetText(FText::FromString(_strValue));
	}
}
