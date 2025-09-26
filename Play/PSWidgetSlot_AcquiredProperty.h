// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_AcquiredProperty.generated.h"

class URichTextBlock;
class UCSImage;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_AcquiredProperty : public UCSWidgetSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	URichTextBlock* PropertyDesc;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* PropertyIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* BackgroundImage;

public:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
};
