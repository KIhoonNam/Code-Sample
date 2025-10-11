// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetSlot_LobbyMenu.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"

void UPSWidgetSlot_LobbyMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();


	if(Image_Icon)
	{
		Image_Icon->SetBrush(m_IconBrush);
	}
	if(Image_SelectIcon)
	{
		Image_SelectIcon->SetBrush(m_IconBrush);
	}
}
