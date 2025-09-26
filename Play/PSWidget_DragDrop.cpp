// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_DragDrop.h"

#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "ProtoSurvivor/UI/Play/PSWidgetSlot_BackPackItem.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"

void UPSWidget_DragDrop::ClearImage()
{
	itemSlot->SlotButton_Image_Icon->SetBrushFromTexture(nullptr);
	itemSlot->Hide();
	itemSlot->itemImage=nullptr;
}

void UPSWidget_DragDrop::testImage()
{
	WidgetReference->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
