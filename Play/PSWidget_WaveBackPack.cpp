// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_WaveBackPack.h"

#include "PaperSprite.h"
#include "PSScrollWidget_CoolDown.h"
#include "PSWidgetSlot_BackPackItem.h"
#include "PSWidgetSlot_CoolDownItem.h"
#include "PSWidget_CoolDown.h"
#include "Animation/WidgetAnimation.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "ProtoSurvivor/UI/Play/PSWidget_DragDrop.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"
#include "UILibrary/Widget/WidgetInterface/CSWidgetScrollBox.h"
#include "UILibrary/Widget/BaseWidget/CSSizeBox.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridSlot.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "Components/UniformGridPanel.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/DataTable/DataManager/PSPropertiesRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSPropertiesRecord.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSWeaponDetailRecord.h"
#include "ProtoSurvivor/Define/PSCommon_BackPack.h"
#include "ProtoSurvivor/Manager/PSAdMobMgr.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "ProtoSurvivor/Manager/PSPurchaseMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/BaseWidget/CSCanvasPanel.h"
#include "UILibrary/Widget/BaseWidget/CSWidgetSwitcher.h"
#include "Components/Overlay.h"

void UPSWidget_WaveBackPack::InitData()
{
	Super::InitData();

	m_fMyCost = g_CSDefineRecordMgr->GetValue_Number("GV_BACKPACK_COST");
    CostReward = 5;
    
    UpdatePlayButtonState();
    UpdateTutorial();
    if (g_PSCharacterMgrValid)
    {
        if (g_PSCharacterMgr->GetPurchasedAdRemove())
        {
            SetAdRemove();
        }
    }
}

void UPSWidget_WaveBackPack::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UPSWidget_WaveBackPack::LinkEvent()
{
	Super::LinkEvent();

    if(g_StageMgrValid)
    {
        g_StageMgr->WaveClear.AddUObject(this,&UPSWidget_WaveBackPack::WaveClear);
    }

    if(g_PSBackPackMgrValid)
    {
        g_PSBackPackMgr->CostDrag.BindUObject(this,&UPSWidget_WaveBackPack::SetCostOutline);
    }
    
    BackPack_EndDelegate.BindDynamic(this,&UPSWidget_WaveBackPack::BackPackAnimationFinished);
    BindToAnimationFinished(BackPack_Animation,BackPack_EndDelegate);
    
	BIND_BUTTON_EVENT(Button_ADRefresh, &UPSWidget_WaveBackPack::OnClicked_ADRefresh);
	BIND_BUTTON_EVENT(Button_RefreshFree, &UPSWidget_WaveBackPack::OnClicked_RefreshFree);
	BIND_BUTTON_EVENT(Button_RefreshSilver, &UPSWidget_WaveBackPack::OnClicked_RefreshSilver);
	BIND_BUTTON_EVENT(Button_ADSilver, &UPSWidget_WaveBackPack::OnClicked_ADSilver);
	BIND_BUTTON_EVENT(Button_Play, &UPSWidget_WaveBackPack::OnClicked_Play);

    if (g_PSPurchaseMgrValid)
    {
        g_PSPurchaseMgr->PurchasedAdRemove.AddUObject(this, &UPSWidget_WaveBackPack::SetAdRemove);
    }
}

void UPSWidget_WaveBackPack::UnLinkEvent()
{
    if(g_StageMgrValid)
    {
        g_StageMgr->WaveClear.RemoveAll(this);
    }
    if(g_PSBackPackMgrValid)
    {
        g_PSBackPackMgr->CostDrag.Unbind();
    }
    if (BackPack_Animation)
    {
        UnbindAllFromAnimationFinished(BackPack_Animation);
    }
    
	UNBIND_BUTTON_EVENT_ALL(Button_ADRefresh);
	UNBIND_BUTTON_EVENT_ALL(Button_RefreshFree);
	UNBIND_BUTTON_EVENT_ALL(Button_Play);

    if (g_PSPurchaseMgrValid)
    {
        g_PSPurchaseMgr->PurchasedAdRemove.RemoveAll(this);
    }
	
	Super::UnLinkEvent();
}

void UPSWidget_WaveBackPack::OnSlotAdded(UPSWidgetSlot_BackPackItem* SlotItem)
{
    if (SlotItem && !SlotItem->OnUpgradeCostChanged.IsBound())
    {
        SlotItem->OnUpgradeCostChanged.AddDynamic(this, &UPSWidget_WaveBackPack::UpdateCostOnUpgrade);
    }
}

void UPSWidget_WaveBackPack::OnSlotRemoved(UPSWidgetSlot_BackPackItem* SlotItem)
{
    if (SlotItem && SlotItem->OnUpgradeCostChanged.IsBound())
    {
        SlotItem->OnUpgradeCostChanged.RemoveDynamic(this, &UPSWidget_WaveBackPack::UpdateCostOnUpgrade);
    }
}

void UPSWidget_WaveBackPack::WaveClear()
{
    if(Button_ADRefresh)
    {
        Button_ADRefresh->SetIsEnabled(true);
    }
    if(Canvas_BackPack)
    {
        Canvas_BackPack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    if (BlockImage)
    {
        BlockImage->SetVisibility(ESlateVisibility::Visible);
    }
    
    ReversBackPackAnimation();
}

void UPSWidget_WaveBackPack::UpdateRefreshButton()
{
    if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
    {
        if(g_PSPropertiesMgrValid)
        {
            int32 buyPrice = 15 * g_PSPropertiesMgr->m_fGlobalRefreshSaleBuff;
            
            if(g_StageMgrValid && WidgetSwitcher_Refresh)
            {
                if(g_StageMgr->GetWaveNumber() == 0)
                {
                    WidgetSwitcher_Refresh->SetActiveWidgetIndex(0);
                }
                else
                {
                    if(PSCharacter->GetSilver() < buyPrice)
                    {
                        WidgetSwitcher_Refresh->SetActiveWidgetIndex(2);
                    }
                    else
                    {
                        WidgetSwitcher_Refresh->SetActiveWidgetIndex(1);
                    }
                }
                TextBlock_RefreshSilver->SetText(FText::FromString(FString::Printf(TEXT("%d"), buyPrice)));
            }
        }
    }
}

void UPSWidget_WaveBackPack::ReorderGrid(UUniformGridPanel* GridPanel, int32 MaxColumns, const FString& PanelName)
{
    if (!GridPanel)
        return;

    TArray<UWidget*> RemainingWidgets;

    for (int32 i = GridPanel->GetChildrenCount() - 1; i >= 0; --i)
    {
        UWidget* ChildWidget = GridPanel->GetChildAt(i);
        if (ChildWidget && ChildWidget->GetVisibility() == ESlateVisibility::Collapsed)
        {
            if (UPSWidgetSlot_BackPackItem* SlotItem = Cast<UPSWidgetSlot_BackPackItem>(ChildWidget))
            {
                OnSlotRemoved(SlotItem);
            }
            GridPanel->RemoveChildAt(i);
        }
        else if (ChildWidget)
        {
            RemainingWidgets.Add(ChildWidget);
        }
    }

    for (int32 i = 0; i < RemainingWidgets.Num(); ++i)
    {
        UWidget* ChildWidget = RemainingWidgets[RemainingWidgets.Num() - 1 - i];
        const int32 Row = i / MaxColumns;
        const int32 Column = i % MaxColumns;

        if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ChildWidget->Slot))
        {
            GridSlot->SetRow(Row);
            GridSlot->SetColumn(Column);
            //UE_LOG(LogTemp, Log, TEXT("%s: 위젯 재배치: 행: %d, 열: %d"), *PanelName, Row, Column);
        }
    }

    GridPanel->InvalidateLayoutAndVolatility();
    UpdatePlayButtonState();
}

bool UPSWidget_WaveBackPack::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UPSWidget_DragDrop* DragDropOperation = Cast<UPSWidget_DragDrop>(InOperation);
    if (!IsValid(DragDropOperation))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cast Returned null"));
        return false;
    }

    const int32 MaxColumns = 5;
    FWeaponStat* WeaponStat = DragDropOperation->m_pWeaponStat;
    if(!WeaponStat) return false;

    bool bIsDroppedInBackPack = false;
    bool bIsDroppedInInventory = false;
    
    // 백팩 드롭
    if (SizeBox_BackPack && GridPanel_BackPack)
    {
        if(WeaponStat)
        {
            if(WeaponStat->m_eTargetType == nTargetType::Cost)
            {
                if(g_PSBackPackMgrValid)
                {
                    g_PSBackPackMgr->OnCostDrag(false);
                }
            }
        }
        const FVector2D DropPosition = SizeBox_BackPack->GetCachedGeometry().AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
        const FVector2D SizeBoxPosition = FVector2D(0.0f, 0.0f);
        const FVector2D SizeBoxSize = SizeBox_BackPack->GetCachedGeometry().GetLocalSize();

        if (DropPosition.X >= SizeBoxPosition.X && DropPosition.X <= SizeBoxPosition.X + SizeBoxSize.X &&
            DropPosition.Y >= SizeBoxPosition.Y && DropPosition.Y <= SizeBoxPosition.Y + SizeBoxSize.Y)
        {
            bIsDroppedInBackPack = true;
            
            const int32 MaxSlots = g_CSDefineRecordMgr->GetValue_Number("GV_BACKPACK_MAX");
            const int32 TotalChildren = GridPanel_BackPack->GetChildrenCount();

            
            if ((TotalChildren >= MaxSlots || m_fMyCost < WeaponStat->m_fCost || DragDropOperation->m_eBackPackType == nBackPackType::BackPack)
                && WeaponStat->m_eTargetType != nTargetType::Cost)
            {
                DragDropOperation->testImage();

                if (TotalChildren >= MaxSlots)
                {
                    UE_LOG(LogTemp, Warning, TEXT("백팩에 추가 불가 (슬롯 가득참)"));
                }
                else if (DragDropOperation->m_eBackPackType == nBackPackType::BackPack)
                {
                    UE_LOG(LogTemp, Warning, TEXT("백팩에서 백팩 안됌."));
                }
                else if (m_fMyCost < WeaponStat->m_fCost)
                {
                    UE_LOG(LogTemp, Warning, TEXT("백팩에 추가 불가 (코스트 부족)"));
                    g_GameMsgMgr->ShowMessageBox("Popup_Not_Cost");
                }
                return false;
            }

            const int32 Row = TotalChildren / MaxColumns;
            const int32 Column = TotalChildren % MaxColumns;

            UClass* SlotClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "S_BackPackItem_BP");
            if (SlotClass)
            {
                UPSWidgetSlot_BackPackItem* NewSlot = CreateWidget<UPSWidgetSlot_BackPackItem>(this, SlotClass);
                if (NewSlot)
                {
                    if(WeaponStat->m_eTargetType == nTargetType::Cost)
                    {
                        m_fMyCost += DragDropOperation->GetCost();
                    }
                    else
                    {
                        GridPanel_BackPack->AddChildToUniformGrid(NewSlot, Row, Column);
                        NewSlot->SetSlotWeaponState(WeaponStat, nBackPackType::BackPack);
                        // 슬롯 추가 시 델리게이트 바인딩
                        OnSlotAdded(NewSlot);
                        m_fMyCost -= DragDropOperation->GetCost();
                    }

                    

                    TextBlock_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d"), int(m_fMyCost))));

                    if (g_PSBackPackMgrValid)
                        g_PSBackPackMgr->SetItemCurrent(GridPanel_BackPack->GetChildrenCount());
                    
                    if(g_SoundMgrValid)
                    {
                        g_SoundMgr->ButtonPlaySound(0.0f,"Sound_Drop_Cue");
                    }

                    if(WeaponStat->m_eTargetType == nTargetType::Near ||WeaponStat->m_eTargetType == nTargetType::Random)
                    {
                        g_PSBackPackMgr->SetNearWeaponAdded(true);
                    }
                    
                    DragDropOperation->ClearImage();
                }
            }
        }
    }
    

    // 인벤토리 드롭
    if (SizeBox_Inventory && GridPanel_Inventory)
    {
        const FVector2D DropPosition = SizeBox_Inventory->GetCachedGeometry().AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
        const FVector2D SizeBoxPosition = FVector2D(0.0f, 0.0f);
        const FVector2D SizeBoxSize = SizeBox_Inventory->GetCachedGeometry().GetLocalSize();


        
        if (DropPosition.X >= SizeBoxPosition.X && DropPosition.X <= SizeBoxPosition.X + SizeBoxSize.X &&
            DropPosition.Y >= SizeBoxPosition.Y && DropPosition.Y <= SizeBoxPosition.Y + SizeBoxSize.Y)
        {
            bIsDroppedInInventory = true;
            
            const int32 MaxSlots = g_CSDefineRecordMgr->GetValue_Number("GV_BACKPACK_MAX");
            const int32 TotalChildren = GridPanel_Inventory->GetChildrenCount();

            if (TotalChildren >= MaxSlots || DragDropOperation->m_eBackPackType == nBackPackType::Inventory)
            {
                DragDropOperation->testImage();

                if (TotalChildren >= MaxSlots)
                {
                    UE_LOG(LogTemp, Warning, TEXT("인벤토리에 슬롯 추가 불가 (슬롯 가득참)."));
                }
                else if (DragDropOperation->m_eBackPackType == nBackPackType::Inventory)
                {
                    UE_LOG(LogTemp, Warning, TEXT("인벤토리에서 인벤토리 안됌."));
                }

                return false;
            }

            const int32 Row = TotalChildren / MaxColumns;
            const int32 Column = TotalChildren % MaxColumns;

            UClass* SlotClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "S_BackPackItem_BP");
            if (SlotClass)
            {
                UPSWidgetSlot_BackPackItem* NewSlot = CreateWidget<UPSWidgetSlot_BackPackItem>(this, SlotClass);
                if (NewSlot)
                {
                    GridPanel_Inventory->AddChildToUniformGrid(NewSlot, Row, Column);
                    NewSlot->SetSlotWeaponState(WeaponStat, nBackPackType::Inventory);
                    m_fMyCost += DragDropOperation->GetCost();
                    TextBlock_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d"), int(m_fMyCost))));
                    if(g_SoundMgrValid)
                    {
                        g_SoundMgr->ButtonPlaySound(0.0f,"Sound_Drop_Cue");
                    }
                    DragDropOperation->ClearImage();
                }
            }
        }
    }

    if (!bIsDroppedInBackPack && !bIsDroppedInInventory)
    {
        UE_LOG(LogTemp, Warning, TEXT("드롭이 허용되지 않은 영역입니다."));
        return false;
    }

    ReorderGrid(GridPanel_BackPack, MaxColumns, TEXT("백팩"));
    ReorderGrid(GridPanel_Inventory, MaxColumns, TEXT("인벤토리"));

    UpdateTutorial();
    
    return true;
}

void UPSWidget_WaveBackPack::GetSilver()
{
    if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
    {
        PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord("SilverGain");
        if (pRecord)
        {
            PSCharacter->AddSilver(pRecord->m_Properties->m_fValue);
        }
    }
    RefreshSilverNumber--;
    if(TextBlock_SilverAD)
    {
        FString strRefreshSilver = TEXT("무료(") + FString::FromInt(RefreshSilverNumber) + TEXT(")");
        TextBlock_SilverAD->SetText(FText::FromString(strRefreshSilver));
    }
    if(RefreshSilverNumber <=0)
    {
        if(Button_ADSilver)
        {
            Button_ADSilver->SetIsEnabled(false);
        }
    }
    if(WidgetSwitcher_Refresh)
        WidgetSwitcher_Refresh->SetActiveWidgetIndex(1);
}

void UPSWidget_WaveBackPack::SetAdRemove()
{
    if (AdIconImage_Refresh)
    {
        AdIconImage_Refresh->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (AdIconImage_Silver)
    {
        AdIconImage_Silver->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UPSWidget_WaveBackPack::SetCostOutline(bool _enable)
{
    if(TextBlock_Cost)
    {
        FSlateFontInfo Outlines =    TextBlock_Cost->GetFont();
        if(_enable)
        {
            Outlines.OutlineSettings.OutlineColor = FLinearColor::Green;
        }
        else
        {
            Outlines.OutlineSettings.OutlineColor = FLinearColor::Black;
        }
        TextBlock_Cost->SetFont(Outlines);
    }
}

void UPSWidget_WaveBackPack::OnClicked_ADRefresh()
{
    if(Button_ADRefresh)
    {
        Button_ADRefresh->SetIsEnabled(false);
    }
	if (g_PSAdMobMgrValid)
	{
	    g_PSAdMobMgr->ShowRewardAd_BackpackRefresh(0, "BackpackRefresh", this);
	}
}

void UPSWidget_WaveBackPack::OnClicked_RefreshFree()
{
    Refresh_Item();
    if(WidgetSwitcher_Refresh)
    WidgetSwitcher_Refresh->SetActiveWidgetIndex(2);
}

void UPSWidget_WaveBackPack::OnClicked_RefreshSilver()
{
    if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
    {
        if(g_PSPropertiesMgrValid)
        {
            if(int32 buyRefresh = 15 * g_PSPropertiesMgr->m_fGlobalRefreshSaleBuff)
            {
                PSCharacter->AddSilver(-buyRefresh);
                Refresh_Item();
                if(PSCharacter->GetSilver() < buyRefresh)
                {
                    if(WidgetSwitcher_Refresh)
                        WidgetSwitcher_Refresh->SetActiveWidgetIndex(2);
                }
            }
        }
    }
}

void UPSWidget_WaveBackPack::OnClicked_ADSilver()
{
    if (g_PSAdMobMgrValid)
    {
        g_PSAdMobMgr->ShowRewardAd_BackpackSilver(0, "BackpackSilver", this);
    }
}

void UPSWidget_WaveBackPack::OnClicked_Play()
{
    PlayBackPackAnimation();

    if (GridPanel_BackPack && W_CoolDownInFo_BP && W_CoolDownInFo_BP->ScrollBox_CoolDown&&g_PSBackPackMgrValid)
    {
        // 기존 슬롯 숨기기 및 데이터 제거
        for (int32 i = W_CoolDownInFo_BP->ScrollBox_CoolDown->GridPanel_CoolTime->GetChildrenCount() - 1; i >= 0; --i)
        {
            UWidget* ChildWidget = W_CoolDownInFo_BP->ScrollBox_CoolDown->GridPanel_CoolTime->GetChildAt(i);
            if (ChildWidget)
            {
                if (UPSWidgetSlot_CoolDownItem* CoolDownSlot = Cast<UPSWidgetSlot_CoolDownItem>(ChildWidget))
                {
                    int32 WeaponKey = i; // 키를 인덱스로 사용
                    g_PSBackPackMgr->m_MapEquipBackPackWeapon.Remove(WeaponKey); // 동기화: 데이터 제거
                    g_PSBackPackMgr->ActiveCoolDownSlots.Remove(CoolDownSlot);  // 배열에서 제거
                    CoolDownSlot->RemoveFromParent(); // 슬롯 제거
                }
            }
        }
        const int32 TotalSlots = GridPanel_BackPack->GetChildrenCount();
        const int32 MaxColumns = 5; // 한 행에 최대 열 수
        float TotalArmorHP = 0.0f; // 방어구 HP 총합 초기화
            
        for (int32 idx = 0; idx < TotalSlots; ++idx)
        {
            UPSWidgetSlot_BackPackItem* SlotItem = Cast<UPSWidgetSlot_BackPackItem>(GridPanel_BackPack->GetChildAt(idx));
            if (SlotItem && SlotItem->m_pWeaponStat)
            {
                UPSWidgetSlot_CoolDownItem* CoolDownSlot = Cast<UPSWidgetSlot_CoolDownItem>(
                    CSUIUtils::LoadWidget(TEXT("S_CoolDown_BP"), GetWorld()));
                if (CoolDownSlot)
                {
                    // 행과 열 계산
                    const int32 Row = idx / MaxColumns;
                    const int32 Column = idx % MaxColumns;

                    W_CoolDownInFo_BP->ScrollBox_CoolDown->GridPanel_CoolTime->AddChildToUniformGrid(CoolDownSlot, Row, Column);

                    CoolDownSlot->m_pWeaponStat = SlotItem->m_pWeaponStat;
                    CoolDownSlot->SetItemImage(CSUIUtils::LoadSprite(SlotItem->m_pWeaponStat->m_strSpriteName));
                    CoolDownSlot->SetSlotIdx(idx);
                    CoolDownSlot->Show();
                    
                    if (!g_PSBackPackMgr->ActiveCoolDownSlots.Contains(CoolDownSlot))
                    {
                        g_PSBackPackMgr->ActiveCoolDownSlots.Add(CoolDownSlot);
                    }
                    
                    int32 WeaponKey = idx; // 키로 슬롯 인덱스 사용
                    g_PSBackPackMgr->m_MapEquipBackPackWeapon.Add(WeaponKey, SlotItem->m_pWeaponStat);

                    UE_LOG(LogTemp, Log, TEXT("CoolDownSlot 업데이트 및 동기화: Key(%d), Name(%s)"), WeaponKey, *SlotItem->m_pWeaponStat->m_strName);
                }
                // 방어구 아이템 HP 총합 계산
                if (SlotItem->m_pWeaponStat->m_eTargetType == nTargetType::Armor || 
                    SlotItem->m_pWeaponStat->m_eTargetType == nTargetType::HP)
                {                    
                    TotalArmorHP += SlotItem->m_pWeaponStat->GetTotalHP();
                }
            }
            
            // 무기 타입 확인 및 버프 설정
            if (g_PSPropertiesMgrValid)
            {
                bool bHasLowTakeDamage = false;
                bool bHasEventCoolTime = false;
                bool bHasRandTakeMelee = false;
                bool bHasRandTakeRange = false;
                
                // 장착된 무기 확인
                for (const TPair<int32, FWeaponStat*>& Pair : g_PSBackPackMgr->m_MapEquipBackPackWeapon)
                {
                    if (Pair.Value)
                    {
                        FString WeaponName = Pair.Value->m_strName;

                        for (nWeaponType::en WeaponType : Pair.Value->m_eWeaponTypes)
                        {
                            switch (WeaponType)
                            {
                            case nWeaponType::LowTakeDamage:
                                if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord("Ring_LowTakeDamage"))
                                {
                                    float ValuePercentage = pRecord->m_Properties->m_fValue / 100.0f;
                                    bHasLowTakeDamage = true;
                                    g_PSPropertiesMgr->AddUniqueProperty<LowTakeDamageProperty>(
                                        g_PSPropertiesMgr->m_arrArmorProperties, 
                                        ArmorPropertyType::LowTakeDamage, 
                                        ValuePercentage, 
                                        pRecord->m_Properties->m_fTriggerValue);
                                    UE_LOG(LogTemp, Log, TEXT("LowTakeDamage 버프 활성화: Weapon(%s)"), *WeaponName);
                                }
                                break;
                            case nWeaponType::EventCoolTime:
                                if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord("Ring_EventCoolTime"))
                                {
                                    float ValuePercentage = pRecord->m_Properties->m_fValue / 100.0f;
                                    bHasEventCoolTime = true;
                                    g_PSPropertiesMgr->AddUniqueProperty<LowTakeDamageProperty>(
                                        g_PSPropertiesMgr->m_arrArmorProperties, 
                                        ArmorPropertyType::EventCoolTime, 
                                        ValuePercentage, 
                                        pRecord->m_Properties->m_fTriggerValue);
                                }
                                break;
                            case nWeaponType::RandTakeMelee:
                                if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord("Helmatt_RandTakeMelee"))
                                {
                                    float ValuePercentage = pRecord->m_Properties->m_fValue / 100.0f;
                                    bHasRandTakeMelee = true;
                                    g_PSPropertiesMgr->AddUniqueProperty<TakeMeleeDamageProperty>(
                                        g_PSPropertiesMgr->m_arrArmorProperties, 
                                        ArmorPropertyType::TakeMeleeDamage, 
                                        ValuePercentage, 
                                        pRecord->m_Properties->m_fTriggerValue);
                                }
                                break;
                            case nWeaponType::RandTakeRange:
                                if(PSPropertiesRecord* pRecord = g_PropertiesRecordMgr->FindRecord("Helmatt_RandTakeRange"))
                                {
                                    float ValuePercentage = pRecord->m_Properties->m_fValue / 100.0f;
                                    bHasRandTakeRange = true;
                                    g_PSPropertiesMgr->AddUniqueProperty<TakeMeleeDamageProperty>(
                                        g_PSPropertiesMgr->m_arrArmorProperties, 
                                        ArmorPropertyType::TakeRangeDamage, 
                                        ValuePercentage, 
                                        pRecord->m_Properties->m_fTriggerValue);
                                }
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }

                if (!bHasLowTakeDamage)
                {
                    g_PSPropertiesMgr->RemoveArmorPropertyByType(ArmorPropertyType::LowTakeDamage);
                }

                if (!bHasEventCoolTime)
                {
                    g_PSPropertiesMgr->RemoveArmorPropertyByType(ArmorPropertyType::EventCoolTime);
                }

                if (!bHasRandTakeMelee)
                {
                    g_PSPropertiesMgr->RemoveArmorPropertyByType(ArmorPropertyType::TakeMeleeDamage);
                }

                if (!bHasRandTakeRange)
                {
                    g_PSPropertiesMgr->RemoveArmorPropertyByType(ArmorPropertyType::TakeRangeDamage);
                }
            }

            
            // 방어구 아이템 총 HP와 캐릭터 ArmorHP를 비교하여 업데이트
            if (APSCharacterBase* pCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
            {
                if (TotalArmorHP != pCharacter->GetArmorHP())
                {
                    float HPPercent = pCharacter->GetCurrentHP() / pCharacter->GetMaxHP();
                    pCharacter->SetArmorHP(0); // ArmorHP 초기화
                    pCharacter->SetArmorHP(TotalArmorHP); // 총 HP 값 적용
                    pCharacter->SetCurrentHP_Heal(pCharacter->GetMaxHP() * HPPercent - pCharacter->GetCurrentHP());

                    UE_LOG(LogTemp, Log, TEXT("총 ArmorHP: %f, 캐릭터 ArmorHP: %f"), TotalArmorHP, pCharacter->GetMaxHP());
                }
            }
        }
    }

    // 전투 시작
    if (g_StageMgrValid)
    {
        g_StageMgr->WaveSetting();
        g_StageMgr->WaveStart();

        if (APSCharacterBase* pCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
        {
            pCharacter->SetCurrentHP_Heal(0.f);
        }
    }
}

void UPSWidget_WaveBackPack::Refresh_Item()
{
    static bool bIsFirstRefresh = true; // 첫 Refresh 여부를 추적
    CostReward -= 1;

    if (GridPanel_Inventory)
    {
        TArray<UWidget*> RemainingWidgets;
        for (int32 i = GridPanel_Inventory->GetChildrenCount() - 1; i >= 0; --i)
        {
            UWidget* ChildWidget = GridPanel_Inventory->GetChildAt(i);
            if (ChildWidget)
            {
                if (UPSWidgetSlot_BackPackItem* SlotItem = Cast<UPSWidgetSlot_BackPackItem>(ChildWidget))
                {
                    OnSlotRemoved(SlotItem);
                }

                GridPanel_Inventory->RemoveChildAt(i);
            }
        }

        const int32 LoopCount = g_CSDefineRecordMgr->GetValue_Number("GV_BACKPACK_REFRESHCOUNT");
        const int32 MaxColumns = 5; // 한 행에 최대 열 수
        int Row = 0;
        int Column = 0;

        // Near 타입 무기 필터링
        TArray<FWeaponStat*> NearWeapons;
        if (g_PSCharacterMgrValid)
        {
            for (auto& Pair : g_PSCharacterMgr->m_mapEquipWeapon)
            {
                if (Pair.Value && Pair.Value->m_eTargetType == nTargetType::Near)
                {
                    NearWeapons.Add(Pair.Value);
                }
            }
        }

        int32 NearSlotIndex = bIsFirstRefresh ? 0 : FMath::RandRange(0, FMath::Min(2, LoopCount - 1)); // 첫 Refresh 또는 무작위 슬롯 선택

        for (int32 idx = 0; idx < LoopCount; ++idx)
        {
            bool isHighWeapon = false;
            UPSWidgetSlot_BackPackItem* NewSlot = Cast<UPSWidgetSlot_BackPackItem>(
                CSUIUtils::LoadWidget(TEXT("S_BackPackItem_BP"), GetWorld()));

            if (NewSlot)
            {
                NewSlot->SetSlotIdx(idx);

                if (g_PSCharacterMgrValid && g_PSBackPackMgrValid)
                {
                    int32 WeaponNum = g_PSCharacterMgr->m_mapEquipWeapon.Num();
                    TArray<int32> arrEquipWeaponKeys;
                    g_PSCharacterMgr->m_mapEquipWeapon.GetKeys(arrEquipWeaponKeys);

                    FWeaponStat* SelectedWeaponStat = nullptr;

                    // Near 무기 강제 포함
                    if (!g_PSBackPackMgr->GetNearWeaponAdded() && idx == NearSlotIndex && NearWeapons.Num() > 0)
                    {
                        SelectedWeaponStat = NearWeapons[FMath::RandRange(0, NearWeapons.Num() - 1)];
            
                    }
                    else if (WeaponNum > 0)
                    {
                        if (CostReward <= 0)
                        {
                            int32 RandomCost = FMath::RandRange(1,3);
                            FString CostTid = TEXT("cost") + FString::FromInt(RandomCost);
                            //Cost 생성
                            if(PSWeaponDetailRecord* pRecord = g_WeaponDetailRecordMgr->FindRecord(CostTid))
                            {
                                SelectedWeaponStat = pRecord->m_WeaponStat;
                            }
                            CostReward = 5;
                        }
                        else
                        {
                            if(RefreshProbabilitySlot("GV_REFRESH_SLOT_COST"))
                            {
                                int32 RandomCost = FMath::RandRange(1,2);
                                FString CostTid = TEXT("cost") + FString::FromInt(RandomCost);
                                //Cost 생성
                                if(PSWeaponDetailRecord* pRecord = g_WeaponDetailRecordMgr->FindRecord(CostTid))
                                {
                                    SelectedWeaponStat = pRecord->m_WeaponStat;
                                }
                            }
                            else
                            {
                                // 랜덤으로 무기 선택
                                int32 RandomIndex = FMath::RandRange(0, WeaponNum - 1);
                                int32 RandomKey = arrEquipWeaponKeys[RandomIndex];

                                SelectedWeaponStat = g_PSCharacterMgr->m_mapEquipWeapon[RandomKey];

                                if(RefreshProbabilitySlot("GV_REFRESH_SLOT_HIGHWEAPON"))
                                {
                                    isHighWeapon = true;
                                }
                            }
                        }
                  

                    }
                    GridPanel_Inventory->AddChildToUniformGrid(NewSlot, Row, Column);
                    if (SelectedWeaponStat)
                    {
                        NewSlot->SetSlotWeaponState(SelectedWeaponStat, nBackPackType::Inventory,isHighWeapon);
                    }
                }

                NewSlot->Show();


                OnSlotAdded(NewSlot);

                Column++;
                if (Column >= MaxColumns)
                {
                    Column = 0;
                    Row++;
                }
            }
        }

        // 첫 Refresh 이후 플래그 업데이트
        if (bIsFirstRefresh)
        {
            bIsFirstRefresh = false;
        }

        // Play 버튼 상태 업데이트
        UpdatePlayButtonState();
        UpdateInventorySlots();

        FString strAmount = FString::Printf(TEXT("%d"), CostReward);
        TextBlock_CostReward->SetText(FText::FromString(strAmount));

        
        GridPanel_Inventory->InvalidateLayoutAndVolatility();
    }
}

void UPSWidget_WaveBackPack::UpdateTutorial()
{
    static bool bWasTutorial = false;

    bool bCurrentTutorial = (GridPanel_BackPack->GetChildrenCount() == 0);

    if (bCurrentTutorial && !bWasTutorial)
    {
        PlayAnimation(Tutorial_Animation);
    }
    else if (!bCurrentTutorial && bWasTutorial)
    {
        PlayAnimationReverse(Tutorial_Animation);
    }

    bWasTutorial = bCurrentTutorial;
}

void UPSWidget_WaveBackPack::PlayBackPackAnimation()
{
    if(BackPack_Animation)
    {
        UnbindAllFromAnimationFinished(BackPack_Animation);
        BackPack_EndDelegate.BindDynamic(this, &UPSWidget_WaveBackPack::BackPackAnimationFinished);
        BindToAnimationFinished(BackPack_Animation, BackPack_EndDelegate);
        
        PlayAnimation(BackPack_Animation);
    }
}

void UPSWidget_WaveBackPack::BackPackAnimationFinished()
{
    if(Canvas_BackPack)
    {
        Canvas_BackPack->SetVisibility(ESlateVisibility::Hidden);
    }
    if (BlockImage)
    {
        BlockImage->SetVisibility(ESlateVisibility::Hidden);
    }

    if (g_PSPauseMgrValid)
    {
        g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::BackPack);
    }
}

void UPSWidget_WaveBackPack::ReversBackPackAnimation()
{
    if (BackPack_Animation)
    {
        UnbindAllFromAnimationFinished(BackPack_Animation);

        BackPack_EndDelegate.BindDynamic(this, &UPSWidget_WaveBackPack::OnReverseBackPackAnimationFinished);
        BindToAnimationFinished(BackPack_Animation, BackPack_EndDelegate);

        // 퍼즈 상태인지 확인
        if (UGameplayStatics::IsGamePaused(GetWorld()))
        {
            // 퍼즈 해제 후 실행되도록 대기
            GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
            {
                WaitForUnpauseAndPlayAnimation();
            });
        }
        else
        {
            // 퍼즈 상태가 아니면 즉시 실행
            PlayAnimation(BackPack_Animation, 0, 1, EUMGSequencePlayMode::Reverse);
            if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
            {
                float Silver = g_CSDefineRecordMgr->GetValue_Number("GV_SILVER_AMOUNT");
                PSCharacter->AddSilver(Silver);
                if(g_PSPropertiesMgrValid)
                {
                    PSCharacter->AddSilver(g_PSPropertiesMgr->m_fGlobalSilverBonusBuff);
                }
            }
            UpdateRefreshButton();
        }
    }
}

void UPSWidget_WaveBackPack::UpdateInventorySlots()
{
    for (UWidget* Widget : GridPanel_BackPack->GetAllChildren())
    {
        if (UPSWidgetSlot_BackPackItem* InventorySlot = Cast<UPSWidgetSlot_BackPackItem>(Widget))
        {
            FWeaponStat* WeaponStat = InventorySlot->GetWeaponStat();
            if (WeaponStat)
            {
                InventorySlot->UpdateSlotData();
            }
        }
    }
}

void UPSWidget_WaveBackPack::WaitForUnpauseAndPlayAnimation()
{
    if (!UGameplayStatics::IsGamePaused(GetWorld()))
    {
        // 퍼즈가 풀리면 애니메이션 실행
        PlayAnimation(BackPack_Animation, 0, 1, EUMGSequencePlayMode::Reverse);
        if (APSCharacterBase* PSCharacter = Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
        {
            float Silver = g_CSDefineRecordMgr->GetValue_Number("GV_SILVER_AMOUNT");
            PSCharacter->AddSilver(Silver);
            if(g_PSPropertiesMgrValid)
            {
                PSCharacter->AddSilver(g_PSPropertiesMgr->m_fGlobalSilverBonusBuff);
            }
        }
        UpdateRefreshButton();
    }
    else
    {
        // 계속 대기
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            WaitForUnpauseAndPlayAnimation();
        });
    }
}


void UPSWidget_WaveBackPack::OnReverseBackPackAnimationFinished()
{
    if(g_PSPauseMgrValid)
    {
        if (g_PSPauseMgrValid)
        {
            g_PSPauseMgr->AddPauseReason(GetWorld(), nPauseType::BackPack);
        }
        Refresh_Item();
    }
}

void UPSWidget_WaveBackPack::UpdateCostOnUpgrade(float InCost)
{
    m_fMyCost += InCost;
    TextBlock_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d"), int(m_fMyCost))));
}

void UPSWidget_WaveBackPack::UpdatePlayButtonState()
{
    bool bHasNearWeapon = false;

    if (GridPanel_BackPack)
    {
        g_PSBackPackMgr->SetNearWeaponAdded(false);
        for (int32 i = 0; i < GridPanel_BackPack->GetChildrenCount(); ++i)
        {
            if (UPSWidgetSlot_BackPackItem* SlotItem = Cast<UPSWidgetSlot_BackPackItem>(GridPanel_BackPack->GetChildAt(i)))
            {
                if (SlotItem->m_pWeaponStat && (SlotItem->m_pWeaponStat->m_eTargetType == nTargetType::Near || SlotItem->m_pWeaponStat->m_eTargetType == nTargetType::Random))
                {
                    bHasNearWeapon = true;
                    g_PSBackPackMgr->SetNearWeaponAdded(true);
                    break;
                }
            }
        }
    }

    if (Button_Play)
    {
        Button_Play->SetIsEnabled(bHasNearWeapon);
    }
}

bool UPSWidget_WaveBackPack::RefreshProbabilitySlot(const FString& _tid)
{
    bool isCorrect = false;

    double fProbability = g_CSDefineRecordMgr->GetValue_Number(_tid);

    double fRandomProbability = UKismetMathLibrary::RandomFloatInRange(0.0f,1.0f);

    UE_LOG(LogTemp,Warning,TEXT("Probability = %lf"),fProbability)
    UE_LOG(LogTemp,Warning,TEXT("RandomProbability = %lf"),fRandomProbability)
    if(fRandomProbability <= fProbability)
    {
        UE_LOG(LogTemp,Warning,TEXT("RandomProbability = %d"),isCorrect)
        isCorrect = true;
    }

    return isCorrect;
}




