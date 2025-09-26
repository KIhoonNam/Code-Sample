// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_CoolDownItem.generated.h"

class UPaperSprite;
class UCSImage;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UCSTextBlock;
class UNiagaraSystemWidget;
struct FWeaponStat;
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_CoolDownItem : public UCSWidgetSlot
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UPaperSprite* itemImage;

	FWeaponStat* m_pWeaponStat;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* SlotImage_CoolDown;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* SlotButton_Image_Border;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_Cost;

	UPROPERTY(meta = (BindWidgetOptional))
	UNiagaraSystemWidget* Niagara_Upgrade;
	
protected:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
public:
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0.f, float fFadeTime = 0.f, float fDisplayTime = 0.f);
	virtual void Hide(float fDelatTime = 0.f, float fFadeTime = 0.f);
	bool WeaponUpgrade();
	void SetItemImage(FWeaponStat* WeaponStat);
	void UpgradeImage(FString& InOutSpriteName);
	void SetItemImage(UPaperSprite* NewItemImage);
	void AdjustCooldownStart();
private:
	bool bIsCoolDownInProgress = false;          // 쿨다운 진행 여부
	bool bIsCooldownAdjusted = false;          // 쿨다운 진행 여부
	float CoolDownElapsedTime = 0.0f;           // 쿨다운 경과 시간
	float CoolDownDuration = 0.4f;              // 쿨다운 총 지속 시간
	float CoolDownStartTime;              // 쿨다운 총 지속 시간
	float LastCombatSpeed = 1.0f; // 마지막으로 추적된 속도 배율
	UMaterialInstanceDynamic* DynamicMaterial;  // 머티리얼 인스턴스

	int32 Number = 0;
};
