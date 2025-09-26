// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_DraggedImage.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"

void UPSWidget_DraggedImage::InitData()
{
	Super::InitData();

	if(Image_Dragged && itemImage)
	{
		Image_Dragged->SetBrushFromSpriteName(itemImage->GetName());
	}

	if(g_SoundMgrValid)
	{
		g_SoundMgr->ButtonPlaySound(0.0f,"Sound_Drag_Cue");
	}
}

void UPSWidget_DraggedImage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_DraggedImage::LinkEvent()
{
	Super::LinkEvent();
}

void UPSWidget_DraggedImage::UnLinkEvent()
{
	Super::UnLinkEvent();
}

FReply UPSWidget_DraggedImage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
	FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    
	return ReplyResult.NativeReply;
}
