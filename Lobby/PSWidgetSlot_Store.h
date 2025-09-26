// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_Store.generated.h"

class UCSWidgetSwitcher;
class UCSTextBlock;
class UCSImage;
class UOverlay;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_Store : public UCSWidgetSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString strGoodsType;
	UPROPERTY(EditAnywhere)
	int32 GoodsNumber;
	UPROPERTY(EditAnywhere)
	FString strGoodsPrice;
	UPROPERTY(EditAnywhere)
	FString strSpriteName;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* NumberText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* PriceText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_SubPriecText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* IconImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Background;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Icon_Price;

	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* Overlay_Timer;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_FreeGoldTime;
public:
	virtual void InitData() override;
	virtual void ReleaseData() override;

	void TryPurchaseDia();
	void TryPurchaseGold();

	nGoodsType::en GetGoodsType() {return m_eGoodsType;}

	void DisableFreeGold();
private:
	void SetSlotInfo();


	
	nGoodsType::en m_eGoodsType = nGoodsType::Max;
};
