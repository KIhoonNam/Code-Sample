// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSPropertiesRecord.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_WeaponInfo.generated.h"

class UCSImage;
class UCSButton;
class UCSTextBlock;
class UOverlay;
class UCSWidgetSwitcher;
class UCSScrollBox;
class UPSWidget_SignalMark;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_WeaponInfo : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Close;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_LevelUp;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Weapon;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Rate;

	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_WeaponName;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Level;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Piece;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Gold;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* Overlay_InfoDesc;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSScrollBox* ScrollBox_Talent;

	UPROPERTY(meta  = (BindWidgetOptional))
	UCSWidgetSwitcher* WidgetSwitcher_Info;

	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* ProgressBar_Piece;
	
	UPROPERTY(meta  = (BindWidgetOptional))
	UPSWidget_SignalMark* W_SignalMark_BP;
public:
	virtual void InitData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

public:
	void SetWeaponInfo(FWeaponStat* pStat);
	void SetWeaponTalent(FWeaponStat* pStat);
	void SetInGameWeaponTalent(FWeaponStat* pStat);
	void HandleBuffType(float BuffValue, const TArray<FString>& arrWeaponTalent, nBuffType::en BuffType, float DefaultValue);
	void HandleAttributeBuff(FWeaponStat* Weapon, const TArray<FString>& arrWeaponTalent);
	void AddTalentToUI(PSPropertiesRecord* pRecord, const FString& strWeaponTalent);
	void SetWeaponLevelUp();
	void SetWeaponDesc();

	int32 GetMaxPiece() { return m_MaxPiece; }
	int32 GetMaxGold() { return m_MaxGold; }
	
	virtual void Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime) override;
private:
	UFUNCTION()
	void OnClick_Close();
	UFUNCTION()
	void OnClick_LevelUp();

	void OnFinishHideAni() override;

	void RefreshTalent();
	
	FString FormatFloatWithCondition(float Value);
	FString ReplacePlaceholders(const FString& Script, const FString& Value, const FString& Percent);
	void UpdateMark(FWeaponStat* WeaponStat);
	void UpdateProgressBar();

private:
	int32 m_Piece = 0;
	int32 m_MaxPiece = 0;
	int32 m_Level = 0;
	int32 m_MaxGold = 0;
	nRateType::en m_eRateType;
	
	FString m_strWeaponTid;

	FWeaponStat* m_WeaponStat;
};
