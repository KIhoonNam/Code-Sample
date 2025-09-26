// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "PSScrollWidget_Stage.generated.h"

class UCSButton;
UCLASS()
class PROTOSURVIVOR_API UPSScrollWidget_Stage : public UCSWidgetScrollBox
{
	GENERATED_BODY()
public:

public:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	
	
	UFUNCTION()
	void InitScrollBox();
	
	void ScrollToSlot(int32 SlotIndex);

public:
	int32 m_CurrentIndex = 0;
};
