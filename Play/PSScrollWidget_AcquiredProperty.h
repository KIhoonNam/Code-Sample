// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "PSScrollWidget_AcquiredProperty.generated.h"

class UCSTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSScrollWidget_AcquiredProperty : public UCSWidgetScrollBox
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* NoPropertyText;

public:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	UFUNCTION()
	void OnPropertySelected();
	FString ReplacePlaceholders(const FString& Script, const FString& Value, const FString& Percent);
};
