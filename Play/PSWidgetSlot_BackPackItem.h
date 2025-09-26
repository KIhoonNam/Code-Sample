// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProtoSurvivor/Define/PSCommon_BackPack.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_BackPackItem.generated.h"

namespace nWeaponType
{
	enum en : int;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeCostChanged, float, NewCost);

class UCSUniformGridPanel;
class UCSButton;
class UTexture2D;
class UPaperSprite;
struct FWeaponStat;
class UCSUniformGridPanel;
class UNiagaraSystemWidget;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_BackPackItem : public UCSWidgetSlot
{
	GENERATED_BODY()
public:
	FOnUpgradeCostChanged OnUpgradeCostChanged;
	
	UPROPERTY()
	UPaperSprite* itemImage;

	FWeaponStat* m_pWeaponStat;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_Cost;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Border;

	UPROPERTY(meta = (BindWidgetOptional))
	UNiagaraSystemWidget* Niagara_Upgrade;

	int32 m_myKey;
	
protected:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void UpgradeImage(FString& InSpriteName);

public:
	virtual void Show(ESlateVisibility visible = ESlateVisibility::SelfHitTestInvisible, float fDelayTime = 0.f, float fFadeTime = 0.f, float fDisplayTime = 0.f);
	virtual void Hide(float fDelatTime = 0.f, float fFadeTime = 0.f);
	void SetCost(int32 cost) { m_iCost = cost; }
	int32 GetCost() { return m_iCost; }
	void SetSlotWeaponState(FWeaponStat* WeaponStat, nBackPackType::en SlotType,bool _isHighWeapon =false);
	void UpdateSlotData();
	FWeaponStat* GetWeaponStat() { return m_pWeaponStat; }
private:
	void SetItemImage(FWeaponStat* WeaponStat);
	void ReorderGrid(UCSUniformGridPanel* GridPanel, int32 MaxColumns);
	bool WeaponUpgrade();
	void StartDragDrop(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	nBackPackType::en m_eBackPackType = nBackPackType::Max;
private:
	int32 m_iCost;

	int32 Number = 0;

	bool bIsDragging = false;
	FVector2D DragStartPosition;
	const float DragThreshold = 10.0f; // 드래그로 판단하는 최소 거리

	float HoldStartTime = -1.0f;
	bool bIsHolding = false;
};
