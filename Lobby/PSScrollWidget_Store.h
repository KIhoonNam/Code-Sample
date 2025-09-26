// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "PSScrollWidget_Store.generated.h"

UCLASS()
class PROTOSURVIVOR_API UPSScrollWidget_Store : public UCSWidgetScrollBox
{
	GENERATED_BODY()

public:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;


	void GetFreeGold(int32 numberGold);
};
