// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_CombatSpeed.generated.h"

class UPSWidget_AdRemove;
class UCSButton;
class UWidgetAnimation;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_CombatSpeed : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Cancel;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Ad;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_Open;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_AdRemove* AdRemoveWidget;

public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0, float fFadeTime = 0, float fDisplayTime = 0) override;

	UFUNCTION()
	void OnClicked_Cancel();
	UFUNCTION()
	void OnClicked_Ad();

	void SetCanChangeCombatSpeed();
};
