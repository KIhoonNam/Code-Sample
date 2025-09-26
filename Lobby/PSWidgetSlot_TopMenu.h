// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_TopMenu.generated.h"

class UOverlay;
class UCSTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_TopMenu : public UCSWidgetSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString strGoodsType;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_EnergyTime;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* Overlay_Timer;
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

public:
	void ChangeGoods();
	bool LogIfEnergyIsLessThanMax(const FString& strEnergy);
	
	int32 RemainingSeconds; //20 * 60; // 20분 (초 단위)
	float AccumulatedDeltaTime = 0.0f;

	void UpdateEnergyTimeDisplay();

	nGoodsType::en GetGoodsType(){return m_eGoodsType;}
private:

private:
	nGoodsType::en m_eGoodsType = nGoodsType::Max;
};
