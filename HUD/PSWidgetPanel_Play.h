// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetPanel.h"
#include "PSWidgetPanel_Play.generated.h"

class UPSWidget_PauseGame;
class UPSWidget_CombatSpeed;
class UProgressBar;
class UCSButton;
class UCSTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetPanel_Play : public UCSWidgetPanel
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* EXPBar;
	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* HPBar;
	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* ArmorBar;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* WaveText;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Pause;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Speed;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* SpeedText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_Silver;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* CurrentHPText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* CurrentArmorText;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_CombatSpeed* CombatSpeedWidget;
	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_PauseGame* PauseGameWidget;

public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	UFUNCTION()
	void OnClicked_Pause();
	UFUNCTION()
	void OnClicked_Speed();

	UFUNCTION()
	void OnCurrentHPChanged(float NewCurrentHP);
	UFUNCTION()
	void OnMaxHPChanged(float NewMaxHP);

	UFUNCTION()
	void OnCurrentArmorChanged(float NewCurrentArmor);
	UFUNCTION()
	void OnMaxArmorChanged(float NewMaxArmor);

	UFUNCTION()
	void OnCurrentEXPChanged(float NewCurrentEXP);
	UFUNCTION()
	void OnMaxEXPChanged(float NewMaxEXP);

	UFUNCTION()
	void OnCurrentWaveChanged(int32 NewCurrentWave);
	UFUNCTION()
	void OnMaxWaveChanged(int32 NewMaxWave);

	UFUNCTION()
	void OnCombatSpeedChanged(float CombatSpeed);
	
	UFUNCTION()
	void OnSilverChanged(int32 NewSilver);
	
	void StartSilverAnimation(int32 StartValue, int32 EndValue);

	void SetCanChangeCombatSpeed(bool CanChangeCombatSpeed);

private:
	void updateCombatSpeed();
	
private:
	float m_fCurrentHP;
	float m_fMaxHP;

	float m_fCurrentArmor;
	float m_fMaxArmor;
    
	float m_fCurrentEXP;
	float m_fMaxEXP;

	int32 m_iCurrentWave;
	int32 m_iMaxWave;

	int32 m_iCurrentSpeed;

	// Silver 애니메이션 변수
	bool bAnimatingSilver = false;                      // 애니메이션 활성화 여부
	float SilverAnimationElapsedTime = 0.f;             // 애니메이션 경과 시간
	float SilverAnimationDuration = 1.f;               // 애니메이션 지속 시간 (초 단위)
	int32 SilverAnimationStartValue = 0;                // 시작 값
	int32 SilverAnimationTargetValue = 0;               // 목표 값

	bool m_bCanChangeCombatSpeed = false;
};
