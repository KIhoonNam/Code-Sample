// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_Equipment.h"
#include "PSScrollWidget_Equipment.h"
#include "PSWidgetSlot_Equipment.h"
#include "PSWidget_MyEquip.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"

void UPSWidget_Equipment::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Equip,&UPSWidget_Equipment::OnClickEquip);
	BIND_BUTTON_EVENT(Button_Info,&UPSWidget_Equipment::OnClickInfo);
}

void UPSWidget_Equipment::UnLinkEvent()
{
	Super::UnLinkEvent();
}

void UPSWidget_Equipment::OnChildItemClicked(UCSUserWidgetBase* _pChildWidget)
{
	Super::OnChildItemClicked(_pChildWidget);


		switch (m_eEquipmentType)
		{
		case nEquipmentType::Equip:
			{
				if(UPSWidgetSlot_Equipment* pEquipmentSlot = Cast<UPSWidgetSlot_Equipment>(_pChildWidget))
				{
					if(Widget_MyEquip)
					{
						if(Widget_MyEquip->TryEquip(pEquipmentSlot->GetWeaponStat()))
						{
							pEquipmentSlot->SetVisibility(ESlateVisibility::Collapsed);
						}
					}

					if(Widget_Equipment)
					{
						Widget_Equipment->OrderListItem();
						Widget_Equipment->OrderListGrid();
					}
				}


				if(UPSWidgetSlot_MyEquip* pMyEquipSlot = Cast<UPSWidgetSlot_MyEquip>(_pChildWidget))
				{
					
					if(Widget_Equipment)
					{
						Widget_Equipment->Unfitted(pMyEquipSlot->GetWeaponStat());
					
					
						if(Widget_MyEquip)
						{
							Widget_MyEquip->SetEmptyEquip(pMyEquipSlot);
						}

						Widget_Equipment->OrderListItem();
						Widget_Equipment->OrderListGrid();
					}

				
				}
			}break;
		case nEquipmentType::Info:
			{
				
			}break;
			default:
				break;
		}
	
	
}

void UPSWidget_Equipment::OnClickEquip()
{
	if(Widget_Equipment && Widget_MyEquip)
	{
		Widget_Equipment->SetEquipmentType(nEquipmentType::Equip);
		Widget_MyEquip->SetEquipmentType(nEquipmentType::Equip);
	}
	m_eEquipmentType = nEquipmentType::Equip;
	if(Button_Info)
	{
		Button_Info->SetVisibility(ESlateVisibility::Visible);
	}

	if(Button_Equip)
	{
		Button_Equip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPSWidget_Equipment::OnClickInfo()
{
	if(Widget_Equipment && Widget_MyEquip)
	{
		Widget_Equipment->SetEquipmentType(nEquipmentType::Info);
		Widget_MyEquip->SetEquipmentType(nEquipmentType::Info);
	}
	m_eEquipmentType = nEquipmentType::Info;
	
	if(Button_Info)
	{
		Button_Info->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(Button_Equip)
	{
		Button_Equip->SetVisibility(ESlateVisibility::Visible);
	}
}
