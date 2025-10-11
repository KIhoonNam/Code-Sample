// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_SignalMark.generated.h"

/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_SignalMark : public UCSUserWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* Jump_Animation;
	void SyncAnimation();
	
protected:
	virtual void InitData() override;

private:
	UPROPERTY()
	UUMGSequencePlayer* AnimationPlayer;

};
