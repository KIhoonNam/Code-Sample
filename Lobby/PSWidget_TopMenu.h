// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_TopMenu.generated.h"
class UCSWidgetScrollBox;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_TopMenu : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSWidgetScrollBox* ScrollBox_Goods;
	
public:
	virtual void InitData() override;
	
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;
private:
	void ChangedGoods(int I, nGoodsType::en En);
};
