// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/Popup/CSWidget_BasePopup.h"
#include "PSWidget_DiaMessagePopup.generated.h"

class UCSTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_DiaMessagePopup : public UCSWidget_BasePopup
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* PopupAnimation;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* StoreDiaText;

public:
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0, float fFadeTime = 0, float fDisplayTime = 0) override;
	virtual void OnClickedOKEvent() override;
};
