// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_StageClear.generated.h"

class UCSImage;
class UCSTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_StageClear : public UCSWidgetSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(meta =(BindWidgetOptional))
	UCSTextBlock* Text_Amount;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Rate;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Weapon;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* CreateSlotAnim;
	
protected:
	virtual void InitData() override;
	
public:
	void SetGold(int32 amount);

	void SetPieceWeapon(FString strTid,int32 amount);

	void GetClearBonus(int32 multiple);
	
	nClearType::en GetClearType(){return m_eClearType;}
	void SetClearType(nClearType::en _eClearType){m_eClearType = _eClearType;}
private:
	nClearType::en m_eClearType = nClearType::Max;

	int32 m_Value = 0;
	FString m_strTid= "";
};
