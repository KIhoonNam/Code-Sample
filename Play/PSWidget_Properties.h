// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_Properties.generated.h"

class UCSImage;
class UCSButton;
class UCSWidgetScrollBox;
class PSPropertiesRecord;
class URichTextBlock;
class UCSVerticalBox;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_Properties : public UCSUserWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta =(BindWidgetOptional))
	UCSWidgetScrollBox* ScrollBox_Properties;

	UPROPERTY(meta =(BindWidgetOptional))
	URichTextBlock* RichTextblock_RefreshNum;
	
	UPROPERTY(meta =(BindWidgetOptional))
	URichTextBlock* RichTextblock_AllNum;

	UPROPERTY(meta =(BindWidgetOptional))
	UCSVerticalBox* VerticalBox_Refresh;
	
	UPROPERTY(meta =(BindWidgetOptional))
	UCSVerticalBox* VerticalBox_All;
	
	void Refresh();
	bool CheckUpgradableWeapon() const;
	void GetAllProperties();

	UFUNCTION()
	void SetAdRemove();

protected:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;

private:
	TSet<PSPropertiesRecord*> SelectedRecords;
	
	UFUNCTION()
	void OnClicked_ADRefresh();
	UFUNCTION()
	void OnClicked_ADAll();
	UFUNCTION()
	void InitProperties();
	
	void ReSetArmorHP();
	
	UPROPERTY(meta =(BindWidgetOptional))
	UCSButton* Button_ADRefresh;
	UPROPERTY(meta =(BindWidgetOptional))
	UCSButton* Button_ADAll;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* AdIconImage_Refresh;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* AdIconImage_All;

public:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* Properties_Animation;

private:
	int32 RefreshNum;
	int32 AllNum;
};
