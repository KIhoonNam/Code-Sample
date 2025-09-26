// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSClearBonusRecord.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_StageClear.generated.h"

class UCSUniformGridPanel;
class UCSButton;
class UCSTextBlock;
class UCSImage;
class UNiagaraSystemWidget;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_StageClear : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSUniformGridPanel* Grid_Result;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSImage* Image_Result;
	
	UPROPERTY(meta =(BindWidgetOptional))
	UCSImage* Image_label;
	
	UPROPERTY(meta =(BindWidgetOptional))
	UCSButton* Button_End;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSButton* Button_Reward;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSTextBlock* Text_Result;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* AdIconImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UNiagaraSystemWidget* Niagara_Fanfare;
	UPROPERTY(meta = (BindWidgetOptional))
	UNiagaraSystemWidget* Niagara_Star;
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual void Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime) override;
public:
	void SetStageClear();
	void GetRewardBonus();

	void CreateGold(PSClearBonusRecord* pRecord,int32* _count);
	void CreatePieceWeapon(PSClearBonusRecord* pRecord,int32* _count);
	void CreateUnlockWeapon(PSClearBonusRecord* pRecord,int32* _count);

	void SetSlotSort();

	UFUNCTION()
	void SetAdRemove();
private:
	UFUNCTION()
	void OnClickEnd();
	UFUNCTION()
	void OnClickReward();
};
