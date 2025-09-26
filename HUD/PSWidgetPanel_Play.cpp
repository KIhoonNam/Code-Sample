// Fill out your copyright notice in the Description page of Project Settings.

#include "PSWidgetPanel_Play.h"

#include "Components/ProgressBar.h"
#include "CoreLibrary/GameGlobal/CSGameGlobal.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "TimerManager.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/UI/Play/PSWidget_CombatSpeed.h"
#include "ProtoSurvivor/UI/Play/PSWidget_PauseGame.h"
#include "Sound/SoundCue.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"

void UPSWidgetPanel_Play::InitData()
{
	Super::InitData();
	
	if (g_SoundMgrValid)
	{
		g_SoundMgr->PlaySound2D("Sound_PlayBGM_Cue");
	}
	
	if(g_PSBackPackMgrValid)
	{
		g_PSBackPackMgr->BackPackWeaponSetting();
	}
	
	if (g_StageMgrValid)
	{
		g_StageMgr->StageSetting();
		OnMaxWaveChanged(g_StageMgr->GetMaxWaveNumber());
		OnCurrentWaveChanged(g_StageMgr->GetWaveNumber());
		if (g_PSCombatMgrValid)
		{
			OnCombatSpeedChanged(g_PSCombatMgr->GetCombatSpeed());
		}
	}

	if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
	{
		OnMaxArmorChanged(PSCharacter->GetMaxArmor());
		OnCurrentArmorChanged(PSCharacter->GetCurrentArmor());

		OnMaxHPChanged(PSCharacter->GetMaxHP());
		OnCurrentHPChanged(PSCharacter->GetCurrentHP());

		OnMaxEXPChanged(PSCharacter->GetMaxEXP());
		OnCurrentEXPChanged(PSCharacter->GetCurrentEXP());

		OnSilverChanged(PSCharacter->GetSilver());
	}

	if (CombatSpeedWidget)
	{
		CombatSpeedWidget->SetParentWidget(this);
		CombatSpeedWidget->Hide();
	}
	if (PauseGameWidget)
	{
		PauseGameWidget->SetParentWidget(this);
		PauseGameWidget->Hide();
	}

	if (g_PSCharacterMgrValid)
	{
		m_bCanChangeCombatSpeed = g_PSCharacterMgr->GetPurchasedAdRemove();
	}
}

void UPSWidgetPanel_Play::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bAnimatingSilver)
	{
		SilverAnimationElapsedTime += InDeltaTime;
		float Alpha = FMath::Clamp(SilverAnimationElapsedTime / SilverAnimationDuration, 0.f, 1.f);

		float InterpolatedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);

		int32 CurrentSilver = FMath::RoundToInt(FMath::Lerp((float)SilverAnimationStartValue, (float)SilverAnimationTargetValue, InterpolatedAlpha));

		if (TextBlock_Silver)
		{
			TextBlock_Silver->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentSilver)));
		}
		if (Alpha >= 1.f)
		{
			bAnimatingSilver = false;
		}
	}
}


void UPSWidgetPanel_Play::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Pause, &UPSWidgetPanel_Play::OnClicked_Pause);
	BIND_BUTTON_EVENT(Button_Speed, &UPSWidgetPanel_Play::OnClicked_Speed);
	
	if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
	{
		PSCharacter->OnPlayerCurrentHPChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnCurrentHPChanged);
		PSCharacter->OnPlayerMaxHPChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnMaxHPChanged);
		PSCharacter->OnPlayerCurrentArmorChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnCurrentArmorChanged);
		PSCharacter->OnPlayerMaxArmorChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnMaxArmorChanged);
		PSCharacter->OnPlayerCurrentEXPChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnCurrentEXPChanged);
		PSCharacter->OnPlayerMaxEXPChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnMaxEXPChanged);
		PSCharacter->OnPlayerSilverChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnSilverChanged);
	}
	
	if (g_StageMgrValid)
	{
		g_StageMgr->OnWaveNumberChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnCurrentWaveChanged);
		g_StageMgr->OnMaxWaveNumberChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnMaxWaveChanged);
	}

	if (g_PSCombatMgrValid)
	{
		g_PSCombatMgr->OnCombatSpeedChanged.AddDynamic(this, &UPSWidgetPanel_Play::OnCombatSpeedChanged);
	}
}

void UPSWidgetPanel_Play::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Pause);
	UNBIND_BUTTON_EVENT_ALL(Button_Speed);
	
	if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
	{
		PSCharacter->OnPlayerCurrentHPChanged.RemoveAll(this);
		PSCharacter->OnPlayerMaxHPChanged.RemoveAll(this);
		PSCharacter->OnPlayerCurrentArmorChanged.RemoveAll(this);
		PSCharacter->OnPlayerMaxArmorChanged.RemoveAll(this);
		PSCharacter->OnPlayerCurrentEXPChanged.RemoveAll(this);
		PSCharacter->OnPlayerMaxEXPChanged.RemoveAll(this);
		PSCharacter->OnPlayerSilverChanged.RemoveAll(this);
	}
	
	if (g_StageMgrValid)
	{
		g_StageMgr->OnWaveNumberChanged.RemoveAll(this);
		g_StageMgr->OnMaxWaveNumberChanged.RemoveAll(this);
	}

	if (g_PSCombatMgrValid)
	{
		g_PSCombatMgr->OnCombatSpeedChanged.RemoveAll(this);
	}
	
	Super::UnLinkEvent();
}

void UPSWidgetPanel_Play::OnClicked_Pause()
{
	if (PauseGameWidget)
	{
		if (g_PSPauseMgrValid)
		{
			g_PSPauseMgr->AddPauseReason(GetWorld(), nPauseType::PauseMenu);
		}
		PauseGameWidget->Show();	
	}
}

void UPSWidgetPanel_Play::OnClicked_Speed()
{
	if (m_bCanChangeCombatSpeed)
	{
		updateCombatSpeed();
	}
	else
	{
		if (g_PSPauseMgrValid)
		{
			g_PSPauseMgr->AddPauseReason(GetWorld(), nPauseType::Speed);
		}
		CombatSpeedWidget->Show();
	}
}

void UPSWidgetPanel_Play::OnCurrentHPChanged(float NewCurrentHP)
{
	m_fCurrentHP = NewCurrentHP;
	if (HPBar)
	{
		HPBar->SetPercent(m_fCurrentHP / m_fMaxHP);
	}
	if (CurrentHPText)
	{
		CurrentHPText->SetText(FText::FromString(FString::Printf(TEXT("%.f"), m_fCurrentHP)));
	}
}

void UPSWidgetPanel_Play::OnMaxHPChanged(float NewMaxHP)
{
	if (HPBar)
	{
		m_fMaxHP = NewMaxHP;
		HPBar->SetPercent(m_fCurrentHP / m_fMaxHP);
	}
}

void UPSWidgetPanel_Play::OnCurrentArmorChanged(float NewCurrentArmor)
{
	m_fCurrentArmor = NewCurrentArmor;
	if (m_fCurrentArmor <= 0.f)
	{
		m_fCurrentArmor = 0.f;
	}
	
	if (ArmorBar)
	{
		ArmorBar->SetPercent(m_fCurrentArmor / m_fMaxArmor);
	}
	if (CurrentArmorText)
	{
		CurrentArmorText->SetText(FText::FromString(FString::Printf(TEXT("%.f"), m_fCurrentArmor)));
	}
}

void UPSWidgetPanel_Play::OnMaxArmorChanged(float NewMaxArmor)
{
	if (ArmorBar)
	{
		m_fMaxArmor = NewMaxArmor;
		ArmorBar->SetPercent(m_fCurrentArmor / m_fMaxArmor);
	}
}

void UPSWidgetPanel_Play::OnCurrentEXPChanged(float NewCurrentEXP)
{
	if (EXPBar)
	{
		m_fCurrentEXP = NewCurrentEXP;
		EXPBar->SetPercent(m_fCurrentEXP / m_fMaxEXP);
	}
}

void UPSWidgetPanel_Play::OnMaxEXPChanged(float NewMaxEXP)
{
	if (EXPBar)
	{
		m_fMaxEXP = NewMaxEXP;
		EXPBar->SetPercent(m_fCurrentEXP / m_fMaxEXP);
	}
}

void UPSWidgetPanel_Play::OnCurrentWaveChanged(int32 NewCurrentWave)
{
	if (WaveText)
	{
		m_iCurrentWave = NewCurrentWave;
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("웨이브 %i / %i"), m_iCurrentWave + 1, m_iMaxWave)));
	}
}

void UPSWidgetPanel_Play::OnMaxWaveChanged(int32 NewMaxWave)
{
	if (WaveText)
	{
		m_iMaxWave = NewMaxWave;
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("웨이브 %i / %i"), m_iCurrentWave + 1, m_iMaxWave)));
	}
}

void UPSWidgetPanel_Play::OnCombatSpeedChanged(float CombatSpeed)
{
	if (SpeedText)
	{
		m_iCurrentSpeed = CombatSpeed;
		SpeedText->SetText(FText::FromString(FString::Printf(TEXT("x%1.0f"), CombatSpeed)));
	}
}

void UPSWidgetPanel_Play::OnSilverChanged(int32 NewSilver)
{
	if (TextBlock_Silver)
	{
		// 현재 Silver 값 추출
		int32 CurrentSilver = FCString::Atoi(*TextBlock_Silver->GetText().ToString());

		// 애니메이션 초기화 (증가/감소 모두 처리)
		if (NewSilver != CurrentSilver)
		{
			StartSilverAnimation(CurrentSilver, NewSilver);
		}
		else
		{
			// 값이 동일하면 애니메이션 없이 즉시 갱신
			TextBlock_Silver->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewSilver)));
		}
	}
}

void UPSWidgetPanel_Play::StartSilverAnimation(int32 StartValue, int32 EndValue)
{
	bAnimatingSilver = true;
	SilverAnimationStartValue = StartValue;
	SilverAnimationTargetValue = EndValue;
	SilverAnimationElapsedTime = 0.f;
	SilverAnimationDuration = 1.f; // 애니메이션 지속 시간 (1초)
}

void UPSWidgetPanel_Play::SetCanChangeCombatSpeed(bool CanChangeCombatSpeed)
{
	m_bCanChangeCombatSpeed = CanChangeCombatSpeed;
	updateCombatSpeed();
}

void UPSWidgetPanel_Play::updateCombatSpeed()
{
	if (g_PSCombatMgrValid)
	{
		m_iCurrentSpeed++;
		int32 maxCombatSpeed = g_CSDefineRecordMgr->GetValue_Number("GV_MAX_COMBAT_SPEED");
		if (m_iCurrentSpeed > maxCombatSpeed)
		{
			m_iCurrentSpeed = 1;
		}
		g_PSCombatMgr->SetCombatSpeed(m_iCurrentSpeed);
	}
}
