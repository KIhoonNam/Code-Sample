// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_MyEquip.generated.h"

struct FWeaponStat;
class UCSImage;
class UCSWidgetSwitcher;
class UCSTextBlock;
class UPSWidget_SignalMark;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_MyEquip : public UCSWidgetSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Rate;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Weapon;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Level;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSWidgetSwitcher* WidgetSwitcher_Equip;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_SignalMark* W_SignalMark_BP;
public:
	void SetWeaponStat(FWeaponStat* WeaponStat);
	void UpdateMark(FWeaponStat* WeaponStat);
	void SetEmptyStat();

	
	FWeaponStat* GetWeaponStat(){return m_pWeaponStat;}
private:
	FWeaponStat* m_pWeaponStat;
	nRateType::en m_eRateType;
};
