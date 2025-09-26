// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_Sound.generated.h"

class UCSButton;
class UCSTextBlock;
class UWidgetAnimation;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_Sound : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Effect;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_BGM;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Effect;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_BGM;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Cancel;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_Open;
	
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0, float fFadeTime = 0, float fDisplayTime = 0) override;
	
	UFUNCTION()
	void OnClicked_Effect();
	UFUNCTION()
	void OnClicked_BGM();
	UFUNCTION()
	void OnClicked_Cancel();

private:
	void setText_Effect();
	void setText_BGM();

private:
	FString m_strMute;
	FString m_strUnmute;
};
