// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSPropertiesRecord.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_Properties.generated.h"

class PSPropertiesRecord;
class URichTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_Properties : public UCSWidgetSlot
{
	GENERATED_BODY()
public:
	FPropertiesData* m_PropertiesData;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* SlotImage_CoolDown;

	UPROPERTY(meta = (BindWidgetOptional))
	URichTextBlock* RichTextBlock_Text_Title;
protected:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

public:
	void SetSlotPropertiesState(TArray<PSPropertiesRecord*>& AvailableRecords);
	void ApplyRandomChanceForBuff(PSPropertiesRecord* pRecord);
	void SelectProperties();
private:
	void HandleDefaultBuff(nBuffType::en BuffType);
	void HandleBackPackBuff(nBuffType::en BuffType);
	float GetCriticalBoostValue() const;
	FString ReplacePlaceholders(const FString& Script, const FString& Value, const FString& Percent);
};
