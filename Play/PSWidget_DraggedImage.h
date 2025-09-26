// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_DraggedImage.generated.h"

class UTexture2D;
class UCSImage;
class UPaperSprite;
UCLASS()
class PROTOSURVIVOR_API UPSWidget_DraggedImage : public UCSUserWidgetBase
{
	GENERATED_BODY()
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	UPROPERTY(meta=(BindWidgetOptional))
	UCSImage* Image_Dragged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "iteImage")
	UPaperSprite* itemImage;
	
};
