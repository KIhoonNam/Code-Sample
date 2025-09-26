// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_PauseGame.generated.h"

class UPSScrollWidget_AcquiredProperty;
class UCSButton;
class UPSWidget_Sound;
class UWidgetAnimation;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_PauseGame : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Cancel;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Lobby;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Sound;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_Sound* SoundWidget;
	UPROPERTY(meta = (BindWidgetOptional))
	UPSScrollWidget_AcquiredProperty* PropertyWidget;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_Open;
	
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0, float fFadeTime = 0, float fDisplayTime = 0) override;
	
	UFUNCTION()
	void OnClicked_Cancel();
	UFUNCTION()
	void OnClicked_Lobby();
	UFUNCTION()
	void OnClicked_Sound();
};
