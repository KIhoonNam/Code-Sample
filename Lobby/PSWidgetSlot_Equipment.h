// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_Equipment.generated.h"

class UCSImage;
class UCSTextBlock;
class UCSCanvasPanel;
class UPSWidget_SignalMark;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_Equipment : public UCSWidgetSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Rate;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Weapon;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSTextBlock* Text_Name;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSTextBlock* Text_Piece;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSTextBlock* Text_Level;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSCanvasPanel* Canvas_Lock;

	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* ProgressBar_Piece;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_SignalMark* W_SignalMark_BP;
	
public:
	void SetWeaponStat(FWeaponStat* WeaponStat);
	void UpdateMark(FWeaponStat* WeaponStat);
	void UpdateProgressBar();

	FWeaponStat* GetWeaponStat(){return m_pWeaponStat;}
private:
	FWeaponStat* m_pWeaponStat;
	nRateType::en m_eRateType;
};
