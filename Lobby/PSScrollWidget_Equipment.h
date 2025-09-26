// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon_Lobby.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "PSScrollWidget_Equipment.generated.h"

class UCSUniformGridPanel;
class UCSButton;

struct compareFName
{
	bool operator()(const FName& lhs, const FName& rhs) const
	{
		return lhs.Compare(rhs) >= 0;
	}
};
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSScrollWidget_Equipment : public UCSWidgetScrollBox
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSUniformGridPanel* Grid_Equipment;

public:
	
	virtual void InitData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual void ReleaseData() override;
public:
	void SetEquipmentType(nEquipmentType::en _eEquipmentType);
	
	void Unfitted(FWeaponStat* pWeaponStat);
	void OrderListItem();
	void OrderListGrid();
private:
	void LoadCompleteDatabase();
	void WeaponLevelUp(const FString& String, int I, int Arg);
	
	void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;

	void ShowAndHideWeapon(ESlateVisibility visible);
private:
	bool m_IsInitData = false;

	nEquipmentType::en m_eEquipmentType = nEquipmentType::Info;

	std::multimap<FName, int32, compareFName> m_WeaponDetailData;
};
