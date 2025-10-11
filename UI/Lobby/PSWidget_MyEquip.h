// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSWidgetSlot_MyEquip.h"
#include "ProtoSurvivor/Define/PSCommon_Lobby.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_MyEquip.generated.h"

class UCSUniformGridPanel;
class UWidgetAnimation;
class UPSWidgetSlot_MyEquip;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_MyEquip : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSUniformGridPanel* Grid_MyEquip;

	UPROPERTY(meta = (BindWidgetAnimOptional),Transient)
	UWidgetAnimation* Anim_Equip;

	UPROPERTY(meta = (BindWidgetAnimOptional),Transient)
	UWidgetAnimation* Anim_Info;

	UPROPERTY()
	TArray<TWeakObjectPtr<UPSWidgetSlot_MyEquip>> m_arrMyEquip;
public:
	virtual void InitData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;
public:
	void LoadCompleteDatabase();
	void OnWeaponLevelup(const FString& String, int level, int piece);
	void SetEquipmentType(nEquipmentType::en _eEquipmentType);

	int32 GetEmptyEquip();
	void SetEmptyEquip(UPSWidgetSlot_MyEquip* pSlot);
	bool TryEquip(FWeaponStat* pWeaponStat);

private:
	nEquipmentType::en m_eEquipmentType = nEquipmentType::Info;


};
