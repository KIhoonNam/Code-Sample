// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_Damage.generated.h"

class UCSTextBlock;
class UWidgetAnimation;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_Damage : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* DamageText;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* PopupAnim;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient)
	UWidgetAnimation* PopupAnimCritical;
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

	UFUNCTION()
	void OnWaveClear();
	UFUNCTION()
	void OnGamePaused(bool isPaused);

	void CreateDamageText(bool _isCritical,int32 _damage);
private:
	float m_fAnimPauseTime;
};
