// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/Popup/CSWidget_BasePopup.h"
#include "PSWidget_BossWavePopup.generated.h"

class UWidgetAnimation;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_BossWavePopup : public UCSWidget_BasePopup
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* PopupAnimation;

public:
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0, float fFadeTime = 0, float fDisplayTime = 0) override;
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
};
