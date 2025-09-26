// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetPanel.h"
#include "PSWidgetPanel_Splash.generated.h"

class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetPanel_Splash : public UCSWidgetPanel
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent OnLogoFinish;
	
		UPROPERTY(meta  = (BindWidgetAnimOptional),Transient)
	UWidgetAnimation* Anim_LOGO;


public:

	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

private:
	UFUNCTION()
	void OnFinishLogo();

	void OnLoadDatabase();

private:
	float m_fTime;
	bool m_IsStart;
};
