// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "PSScrollWidget_CharacterStore.generated.h"

class UCSUniformGridPanel;
class UPSWidgetSlot_CharacterStatStore;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSScrollWidget_CharacterStore : public UCSWidgetScrollBox
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSUniformGridPanel* Grid_CharacterStore;

public:
	virtual void InitData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	TArray<UPSWidgetSlot_CharacterStatStore*> GetSlots();

	void RefreshSlot();
};
