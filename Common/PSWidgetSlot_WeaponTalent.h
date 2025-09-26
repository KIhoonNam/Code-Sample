// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_WeaponTalent.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_WeaponTalent : public UCSWidgetSlot
{
	GENERATED_BODY()

	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	URichTextBlock* Text_Talent;


public:
	void SetEnableTalent(bool _isEnable,const FString& _text,const FString& _tid);
	void RefreshEnableTalent();
	const FString& GetTalentTid(){return m_strTalentTid;}
	
private:
	FString m_strTalentTid;

};
