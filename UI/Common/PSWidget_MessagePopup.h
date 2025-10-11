// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/Popup/CSWidget_BasePopup.h"
#include "PSWidget_MessagePopup.generated.h"

class UCSTextBlock;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_MessagePopup : public UCSWidget_BasePopup
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Title;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Desc;


public:
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0.f, float fFadeTime = 0.f, float fDisplayTime = 0.f);
};
