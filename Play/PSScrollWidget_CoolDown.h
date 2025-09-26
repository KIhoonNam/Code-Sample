// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "PSScrollWidget_CoolDown.generated.h"

class UCSUniformGridPanel;
UCLASS()
class PROTOSURVIVOR_API UPSScrollWidget_CoolDown : public UCSWidgetScrollBox
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSUniformGridPanel* GridPanel_CoolTime;
	
protected:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
};
