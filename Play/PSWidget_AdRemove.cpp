// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_AdRemove.h"

#include "PSWidget_CombatSpeed.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"

void UPSWidget_AdRemove::InitData()
{
	Super::InitData();
}

void UPSWidget_AdRemove::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_AdRemove::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Pay, &UPSWidget_AdRemove::OnClicked_Pay);
	BIND_BUTTON_EVENT(Button_Cancel, &UPSWidget_AdRemove::OnClicked_Cancel);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.AddUObject(this, &UPSWidget_AdRemove::ApplyAdRemove);
	}
}

void UPSWidget_AdRemove::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Pay);
	UNBIND_BUTTON_EVENT_ALL(Button_Cancel);

	if (g_PSPurchaseMgrValid)
	{
		g_PSPurchaseMgr->PurchasedAdRemove.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidget_AdRemove::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (Animation_Open)
	{
		PlayAnimation(Animation_Open);
	}
}

void UPSWidget_AdRemove::OnClicked_Cancel()
{
	Hide();
}

void UPSWidget_AdRemove::OnClicked_Pay()
{
	if (g_PSPurchaseMgrValid)
	{
		FString adremove = g_CSDefineRecordMgr->GetValue_String("product_adremove");
		g_PSPurchaseMgr->Purchase_AdRemove(0, adremove);
	}
}

void UPSWidget_AdRemove::ApplyAdRemove()
{
	if (UPSWidget_CombatSpeed* pCombatSpeedWidget = Cast<UPSWidget_CombatSpeed>(GetParentWidget()))
	{
		pCombatSpeedWidget->SetCanChangeCombatSpeed();
	}
}
