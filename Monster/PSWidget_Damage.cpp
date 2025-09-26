// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_Damage.h"

#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

void UPSWidget_Damage::InitData()
{
	Super::InitData();

	m_fAnimPauseTime = 0.f;
}

void UPSWidget_Damage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_Damage::LinkEvent()
{
	Super::LinkEvent();

	if (g_StageMgrValid)
	{
		g_StageMgr->WaveClear.AddUObject(this, &UPSWidget_Damage::OnWaveClear);
	}
	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->OnGamePaused.AddUObject(this, &UPSWidget_Damage::OnGamePaused);
	}
}

void UPSWidget_Damage::UnLinkEvent()
{
	if (g_StageMgrValid)
	{
		g_StageMgr->WaveClear.RemoveAll(this);
	}
	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->OnGamePaused.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidget_Damage::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	
	RemoveFromParent();
}

void UPSWidget_Damage::OnWaveClear()
{
	RemoveFromParent();
}

void UPSWidget_Damage::OnGamePaused(bool isPaused)
{
	if (isPaused)
	{
		if (IsAnimationPlaying(PopupAnim))
		{
			m_fAnimPauseTime = PauseAnimation(PopupAnim);
		}
	}
	else
	{
		if (!IsAnimationPlaying(PopupAnim))
		{
			if (g_PSCombatMgrValid)
			{
				PlayAnimation(PopupAnim, m_fAnimPauseTime, 1, EUMGSequencePlayMode::Forward, g_PSCombatMgr->GetCombatSpeed());
			}
		}
	}
}

void UPSWidget_Damage::CreateDamageText(bool _isCritical, int32 _damage)
{
	if(DamageText)
	{


		FString strDamage = FString::Printf(TEXT("%d"), _damage);
		DamageText->SetText(FText::FromString(strDamage));
	}
	if (g_PSCombatMgrValid)
	{
		if(_isCritical)
		{
			DamageText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
			DamageText->SetRenderScale(FVector2D(1.5f));

			FSlateFontInfo Font = DamageText->GetFont();
			Font.Size = 50;
			Font.OutlineSettings.OutlineSize = 4;

			DamageText->SetFont(Font);

			if(PopupAnim)
			{
				PlayAnimation(PopupAnimCritical, 0.f, 1, EUMGSequencePlayMode::Forward, g_PSCombatMgr->GetCombatSpeed());
			}
		}
		else
		{
			if(PopupAnim)
			{
				PlayAnimation(PopupAnim, 0.f, 1, EUMGSequencePlayMode::Forward, g_PSCombatMgr->GetCombatSpeed());
			}
		}

	}

	
	
}


