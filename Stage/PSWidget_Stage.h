// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_Stage.generated.h"

class UPSWidget_AdRemove;
class UPSWidget_Sound;
class UPSWidget_StageInfo;
class UPSScrollWidget_Stage;
class UCSImage;
class UCSButton;
class UCSTextBlock;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_Stage : public UCSUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Start;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Bonus;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Next;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Previous;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_First;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Last;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_Sound;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSButton* Button_AdRemove;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* StageNameText;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* HighWaveText;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSScrollWidget_Stage* ScrollBox_Stage;

	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_StageInfo* Widget_StageInfo;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_Sound* SoundWidget;
	UPROPERTY(meta = (BindWidgetOptional))
	UPSWidget_AdRemove* AdRemoveWidget;

public:
	virtual void InitData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	
	UFUNCTION()
	void OnClicked_Start();
	
	UFUNCTION()
	void OnClicked_Next();
	UFUNCTION()
	void OnClicked_Previous();
	
	UFUNCTION()
	void OnClicked_First();
	UFUNCTION()
	void OnClicked_Last();
	
	UFUNCTION()
	void OnClicked_Sound();
	UFUNCTION()
	void OnClicked_AdRemove();

	UFUNCTION()
	void OnClicked_Bonus();
	
	UFUNCTION()
	void InitStageWidget();
	
	UFUNCTION()
	void ApplyAdRemove();

	void SetStageText();
	void SetCurrentStage(int32 CurrentStage);
	bool IsAllEquip();
	
private:
	void setButtonEnabled();

private:
	int32 m_iCurrentStage;
	int32 m_iMaxOpenStage = 1;
	int32 m_iMaxStage;
};
