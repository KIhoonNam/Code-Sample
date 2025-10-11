// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_WeaponInfoDesc.generated.h"

class UCSVerticalBox;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_WeaponInfoDesc : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSVerticalBox* VerticalBox_Info;
public:
	void SettingInfoDesc(TArray<FString> _arrStrDesc);


};
