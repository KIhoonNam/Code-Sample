// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_Character.generated.h"

class UPSScrollWidget_CharacterStore;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_Character : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UPSScrollWidget_CharacterStore* ScrollWidget_CharacterStore;


	
public:
	virtual void InitData() override;
	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;

	
};
