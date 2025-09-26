// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_HPBar.h"

#include "ProtoSurvivor/Character/PSBossMonster.h"
#include "Components/ProgressBar.h"

void UPSWidget_HPBar::InitData()
{
	Super::InitData();
}

void UPSWidget_HPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_HPBar::LinkEvent()
{
	Super::LinkEvent();

	if (m_pOwningBossMonster)
	{
		m_pOwningBossMonster->OnBossCurrentHPChanged.AddDynamic(this, &UPSWidget_HPBar::OnCurrentHPChanged);
	}
}

void UPSWidget_HPBar::UnLinkEvent()
{
	if (m_pOwningBossMonster)
	{
		m_pOwningBossMonster->OnBossCurrentHPChanged.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidget_HPBar::OnCurrentHPChanged(float NewCurrentHP)
{
	if (HPBar)
	{
		m_fCurrentHP = NewCurrentHP;
		HPBar->SetPercent(m_fCurrentHP / m_fMaxHP);
	}
}

void UPSWidget_HPBar::SetOwningBossMonster(APSBossMonster* BossMonster)
{
	if (BossMonster)
	{
		m_pOwningBossMonster = BossMonster;
		m_fMaxHP = m_pOwningBossMonster->GetCurrentHP();
		OnCurrentHPChanged(m_fMaxHP);
	}
}

