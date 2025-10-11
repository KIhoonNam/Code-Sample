// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_BossWavePopup.h"

#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"

void UPSWidget_BossWavePopup::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (PopupAnimation)
	{
		if (g_PSCombatMgrValid)
		{
			PlayAnimation(PopupAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, g_PSCombatMgr->GetCombatSpeed());
		}
	}
	if (g_SoundMgrValid)
	{
		g_SoundMgr->EffectPlaySound(0.f, "Sound_Siren_Cue");
	}
}

void UPSWidget_BossWavePopup::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	RemoveFromParent();
}
