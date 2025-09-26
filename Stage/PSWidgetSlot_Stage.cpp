// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_Stage.h"

#include "UILibrary/Widget/BaseWidget/CSButton.h"

void UPSWidgetSlot_Stage::InitData()
{
	Super::InitData();

	SlotButton->SetTouchMethod(EButtonTouchMethod::Type::PreciseTap);
}

void UPSWidgetSlot_Stage::ReleaseData()
{
	Super::ReleaseData();
}

void UPSWidgetSlot_Stage::LinkEvent()
{
	Super::LinkEvent();
}

void UPSWidgetSlot_Stage::UnLinkEvent()
{
	Super::UnLinkEvent();
}
