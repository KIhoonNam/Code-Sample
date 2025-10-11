// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_Character.h"
#include "PSScrollWidget_CharacterStore.h"
#include "PSWidgetSlot_CharacterStatStore.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"

void UPSWidget_Character::InitData()
{
	Super::InitData();
}

void UPSWidget_Character::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);

	if(UPSWidgetSlot_CharacterStatStore* pSlot = Cast<UPSWidgetSlot_CharacterStatStore>(_pChildWidget))
	{
		pSlot->PlayClickAnimation();

		int32 currentGold = g_PSCharacterMgr->GetGold();

		pSlot->SellStat(currentGold);
	}
}


