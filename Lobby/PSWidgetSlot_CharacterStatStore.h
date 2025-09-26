// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_CharacterStatStore.generated.h"

class UWidgetAnimation;
class UCSTextBlock;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_CharacterStatStore : public UCSWidgetSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString strName;

	UPROPERTY(EditAnywhere)
	FString strTid;
	
	UPROPERTY(meta = (BindWidgetAnimOptional),Transient)
	UWidgetAnimation* Anim_Click;

	UPROPERTY(meta = (BindWidget))
	UCSTextBlock* Text_Stat;

	UPROPERTY(meta = (BindWidget))
	UCSTextBlock* Text_Name;

	UPROPERTY(meta = (BindWidget))
	UCSTextBlock* Text_Level;
	
	UPROPERTY(meta = (BindWidget))
	UCSTextBlock* Text_Gold;
public:
	virtual void InitData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	
	void PlayClickAnimation();

	void RefreshText();

	void RefreshStat();

	void SellStat(int32 _gold);
private:
	int32 m_LevelNumber;
	float m_fStat;
	int32 m_Gold;
	nCharacterStat::en m_eCharacterStat = nCharacterStat::Max;
	nCharacterStatValue::en m_eCharacterValue = nCharacterStatValue::Max;
	
};
