#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSUserWidgetBase.h"
#include "PSWidget_WaveBackPack.generated.h"

class UUniformGridPanel;
class UCSImage;
class UCSScrollBox;
class UCSButton;
class UCSWidgetScrollBox;
class UPSWidgetSlot_BackPackItem;
class UCSSizeBox;
class UCSUniformGridPanel;
class UTexture2D;
struct FWeaponStat;
class UCSCanvasPanel;
class UCSTextBlock;
class UPSWidget_CoolDown;
class UCSWidgetSwitcher;
class UOverlay;

UCLASS()
class PROTOSURVIVOR_API UPSWidget_WaveBackPack : public UCSUserWidgetBase
{
	GENERATED_BODY()
public:	
	FWeaponStat* m_pWeaponStat;

	UPROPERTY(meta=(BindWidgetOptional))
	UPSWidget_CoolDown* W_CoolDownInFo_BP;
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCSCanvasPanel* Canvas_BackPack;
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCSUniformGridPanel* GridPanel_BackPack;
	UPROPERTY(meta=(BindWidgetOptional))
	UCSUniformGridPanel* GridPanel_Inventory;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSWidgetSwitcher* WidgetSwitcher_Refresh;
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCSButton* Button_ADRefresh;
	UPROPERTY(meta=(BindWidgetOptional))
	UCSButton* Button_RefreshFree;
	UPROPERTY(meta=(BindWidgetOptional))
	UCSButton* Button_RefreshSilver;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_RefreshSilver;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_CostReward;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_SilverAD;
	UPROPERTY(meta=(BindWidgetOptional))
	UCSButton* Button_ADSilver;
	UPROPERTY(meta=(BindWidgetOptional))
	UCSButton* Button_Play;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSSizeBox* SizeBox_BackPack;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSSizeBox* SizeBox_Inventory;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSTextBlock* TextBlock_Cost;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* BackPack_Animation;
	FWidgetAnimationDynamicEvent BackPack_EndDelegate;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* BlockImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* AdIconImage_Refresh;
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* AdIconImage_Silver;

	UPROPERTY(meta = (BindWidgetOptional))
	UOverlay* Overlay_Tutorial;
	
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI", meta = (BindWidgetAnim))
	UWidgetAnimation* Tutorial_Animation;

public:
	void Refresh_Item();
	void UpdateTutorial();
	UFUNCTION()
	void UpdateCostOnUpgrade(float InCost);
	void GetSilver();

	UFUNCTION()
	void SetAdRemove();


	void SetCostOutline(bool _enable);
protected:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
	void OnSlotAdded(UPSWidgetSlot_BackPackItem* SlotItem);
	void OnSlotRemoved(UPSWidgetSlot_BackPackItem* SlotItem);
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
private:
	UFUNCTION()
	void ReorderGrid(UUniformGridPanel* GridPanel, int32 MaxColumns, const FString& PanelName);
	UFUNCTION()
	void OnClicked_ADRefresh();
	UFUNCTION()
	void OnClicked_RefreshFree();
	UFUNCTION()
	void OnClicked_RefreshSilver();
	UFUNCTION()
	void OnClicked_ADSilver();
	UFUNCTION()
	void OnClicked_Play();
	UFUNCTION()
	void PlayBackPackAnimation();
	UFUNCTION()
	void BackPackAnimationFinished();
	UFUNCTION()
	void ReversBackPackAnimation();
	void UpdateInventorySlots();
	void WaitForUnpauseAndPlayAnimation();
	UFUNCTION()
	void OnReverseBackPackAnimationFinished();
	UFUNCTION()
	void WaveClear();
	void UpdateRefreshButton();
	void UpdatePlayButtonState();

	bool RefreshProbabilitySlot(const FString& _tid);
private:
	float m_fMyCost;

	bool bIsDragging = false;
	FVector2D DragStartPosition;
	const float DragThreshold = 10.0f; // 드래그로 판단하는 최소 거리
	int32 CostReward;

	int32 RefreshSilverNumber = 10;
};
