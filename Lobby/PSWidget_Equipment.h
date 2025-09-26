// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon_Lobby.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_Equipment.generated.h"

class UCSButton;
class UPSScrollWidget_Equipment;
class UPSWidget_MyEquip;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_Equipment : public UCSUserWidgetBase
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Equip;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Info;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSScrollWidget_Equipment* Widget_Equipment;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_MyEquip* Widget_MyEquip;


public:
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;
private:
	UFUNCTION()
	void OnClickEquip();
	UFUNCTION()
	void OnClickInfo();


private:
	nEquipmentType::en m_eEquipmentType = nEquipmentType::Info;
};
