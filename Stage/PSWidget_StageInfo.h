// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_StageInfo.generated.h"

class UCSButton;
class UCSTextBlock;
class UCSUniformGridPanel;
class PSClearBonusRecord;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidget_StageInfo : public UCSUserWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_SingleOk;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* Text_Stage;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSUniformGridPanel* Grid_BonusInfo;
public:

	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	
public:
	void CreateBonusInfo(int32 _stageNumber);


private:
	void CreateGold(PSClearBonusRecord* pRecord,int32* _count);
	void CreatePieceWeapon(PSClearBonusRecord* pRecord,int32* _count);
	void CreateUnlockWeapon(PSClearBonusRecord* pRecord,int32* _count);
	
private:
	UFUNCTION()
	void OnClicked_OK();
};
