// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_WeaponInfoDesc.h"

#include "PSWidgetSlot_WeaponInfo.h"
#include "UILibrary/Widget/BaseWidget/CSVerticalBox.h"

void UPSWidget_WeaponInfoDesc::SettingInfoDesc(TArray<FString> _arrStrDesc)
{
	if(VerticalBox_Info)
	{
		int32 Count = 0;
		for(UWidget* pWidget : VerticalBox_Info->GetAllChildren())
		{
			if(UPSWidgetSlot_WeaponInfo* pSlot = Cast<UPSWidgetSlot_WeaponInfo>(pWidget))
			{
				if(_arrStrDesc.IsValidIndex(Count))
				{
					pSlot->SetValueText(_arrStrDesc[Count]);
					Count++;
				}
				else
				{
					break;
				}
			}
		}
	}
}
