// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_WeaponTalent.h"

#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "Components/RichTextBlock.h"



void UPSWidgetSlot_WeaponTalent::SetEnableTalent(bool _isEnable, const FString& _text, const FString& _tid)
{
	m_strTalentTid = _tid;

	if(SlotButton)
	{
		SlotButton->SetIsEnabled(_isEnable);
	}

	if(Text_Talent)
	{
		Text_Talent->SetText(FText::FromString(_text));
	}
}

void UPSWidgetSlot_WeaponTalent::RefreshEnableTalent()
{
	if(SlotButton)
	{
		SlotButton->SetIsEnabled(true);
	}
}
