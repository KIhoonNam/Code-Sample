// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_HPBar.generated.h"

class UProgressBar;
class APSBossMonster;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_HPBar : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* HPBar;

public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	UFUNCTION()
	void OnCurrentHPChanged(float NewCurrentHP);

	void SetOwningBossMonster(APSBossMonster* BossMonster);

private:
	float m_fCurrentHP;
	float m_fMaxHP;

	APSBossMonster* m_pOwningBossMonster;
};
