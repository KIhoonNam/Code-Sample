// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetPanel.h"
#include "PSWidgetPanel_Respawn.generated.h"

class UCSWidgetSwitcher;
class UCSButton;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetPanel_Respawn : public UCSWidgetPanel
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSWidgetSwitcher* ButtonSwitcher;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Cancel;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Respawn;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Ad;

public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0.f, float fFadeTime = 0.f, float fDisplayTime = 0.f) override;

	UFUNCTION()
	void OnClicked_Cancel();
	UFUNCTION()
	void OnClicked_Respawn();

	UFUNCTION()
	void ApplyAdRemove();
};