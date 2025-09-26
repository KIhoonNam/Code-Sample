// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidgetSlot_WeaponInfo.generated.h"

class UCSTextBlock;
class UCSImage;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_WeaponInfo : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Value;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Desc;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Value;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSWidgetSlot", DisplayName = "Text Title")
	FName m_strTextTitle = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CSWidgetSlot", DisplayName = "Slot Image")
	FSlateBrush m_Image;

public:
	virtual void SynchronizeProperties() override;

public:
	void SetValueText(const FString& _strValue);
};
