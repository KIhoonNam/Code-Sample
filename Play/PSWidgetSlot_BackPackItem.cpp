#include "PSWidgetSlot_BackPackItem.h"

#include "PaperSprite.h"
#include "PSWidget_DraggedImage.h"
#include "PSWidget_WaveBackPack.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/UniformGridSlot.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/UI/Play/PSWidget_DragDrop.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "UILibrary/Widget/BaseWidget/CSCanvasPanel.h"
#include "UILibrary/Widget/BaseWidget/CSImage.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"
#include "UILibrary/Widget/BaseWidget/CSUniformGridPanel.h"
#include "NiagaraSystemWidget.h"

void UPSWidgetSlot_BackPackItem::InitData()
{
    Super::InitData();





}

void UPSWidgetSlot_BackPackItem::ReleaseData()
{
    if (m_pWeaponStat)
    {
        delete m_pWeaponStat;
        m_pWeaponStat = nullptr;
    }

    Super::ReleaseData();
}


void UPSWidgetSlot_BackPackItem::SetSlotWeaponState(FWeaponStat* WeaponStat, nBackPackType::en SlotType,bool _isHighWeapon)
{
    if (WeaponStat)
    {
        m_eBackPackType = SlotType;

        // 기존 데이터 삭제
        if (m_pWeaponStat)
        {
            delete m_pWeaponStat;
        }

        // 새로운 데이터 복사
        m_pWeaponStat = new FWeaponStat(*WeaponStat);

        // 이미지 및 텍스트 업데이트
        SetItemImage(m_pWeaponStat);

        if (TextBlock_Cost && m_pWeaponStat)
        {
            TextBlock_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d"), int(m_pWeaponStat->m_fCost))));
        }

        if(_isHighWeapon)
        {
            WeaponUpgrade();
            if(g_SoundMgrValid)
            {
                g_SoundMgr->ButtonPlaySound(0.0f,"Sound_WeaponUpgrade_Cue");
            }
            if(Niagara_Upgrade)
            {
                Niagara_Upgrade->ActivateSystem(true);
            }
        }

        if(Image_Border->IsVisible())
        {
            Image_Border->SetBrushFromSpriteName(m_pWeaponStat->m_strRateSpriteName);
        }
        if(IsValid(itemImage))
        {
            SlotButton_Image_Icon->SetBrushFromSpriteName(itemImage->GetName());
        }
    }
}

void UPSWidgetSlot_BackPackItem::UpdateSlotData()
{
    if (itemImage->GetName() != m_pWeaponStat->m_strSpriteName)
    {
        SetItemImage(m_pWeaponStat);

        if (SlotButton_Image_Icon)
        {
            SlotButton_Image_Icon->SetBrushFromSpriteName(m_pWeaponStat->m_strSpriteName);
        }
    }
}

void UPSWidgetSlot_BackPackItem::SetItemImage(FWeaponStat* WeaponStat)
{
    if (UPaperSprite* pTexture = CSUIUtils::LoadSprite(WeaponStat->m_strSpriteName))
    {
        if (IsValid(pTexture))
        {
            itemImage = pTexture;
        }
    }
}

void UPSWidgetSlot_BackPackItem::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
    Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);
}

void UPSWidgetSlot_BackPackItem::Hide(float fDelatTime, float fFadeTime)
{
    Super::Hide(fDelatTime, fFadeTime);
}

void UPSWidgetSlot_BackPackItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsHolding)
    {
        float CurrentTime = GetWorld()->GetRealTimeSeconds();
        float ElapsedTime = CurrentTime - HoldStartTime;

        if (ElapsedTime >= 1.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("툴팁 표시!"));
            bIsHolding = false;
        }
    }
}

void UPSWidgetSlot_BackPackItem::LinkEvent()
{
    Super::LinkEvent();
}

void UPSWidgetSlot_BackPackItem::UnLinkEvent()
{
    Super::UnLinkEvent();
}

FReply UPSWidgetSlot_BackPackItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = false;
        DragStartPosition = InMouseEvent.GetScreenSpacePosition();

        HoldStartTime = GetWorld()->GetRealTimeSeconds();
        bIsHolding = true;
    }

    return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}


void UPSWidgetSlot_BackPackItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    if (IsValid(itemImage))
    {
        UClass* pDragdropexampleClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "W_Dragdropexample_BP");
        if (pDragdropexampleClass)
        {
            // 드래그된 아이템을 생성
            UPSWidget_DraggedImage* pDragdropexampleBot = CreateWidget<UPSWidget_DraggedImage>(this, pDragdropexampleClass);
            UPSWidget_DragDrop* DragDropOperation = NewObject<UPSWidget_DragDrop>();
            if(m_pWeaponStat)
            {
                if(m_pWeaponStat->m_eTargetType == nTargetType::Cost)
                {
                    if(g_PSBackPackMgrValid)
                    {
                        g_PSBackPackMgr->OnCostDrag(true);
                    }
                }
            }
            if (pDragdropexampleBot)
            {
                pDragdropexampleBot->itemImage = itemImage;
            }

            if (DragDropOperation)
            {
                DragDropOperation->WidgetReference = this;
                DragDropOperation->DefaultDragVisual = pDragdropexampleBot;
                DragDropOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
                DragDropOperation->Pivot = EDragPivot::CenterCenter;
                DragDropOperation->itemImage = itemImage;
                DragDropOperation->itemSlot = this;
                DragDropOperation->m_eBackPackType = m_eBackPackType;
                DragDropOperation->m_pWeaponStat = m_pWeaponStat;
                DragDropOperation->myKey = m_myKey;

                OutOperation = DragDropOperation;
                
                SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

bool UPSWidgetSlot_BackPackItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UPSWidget_DragDrop* DragDropOperation = Cast<UPSWidget_DragDrop>(InOperation);

    if (DragDropOperation && DragDropOperation->m_pWeaponStat && m_pWeaponStat)
    {
        if(DragDropOperation->m_pWeaponStat)
        {
            if(DragDropOperation->m_pWeaponStat->m_eTargetType == nTargetType::Cost)
            {
                if(g_PSBackPackMgrValid)
                {
                    g_PSBackPackMgr->OnCostDrag(false);
                }
            }
        }
        UE_LOG(LogTemp, Log, TEXT("드랍 대상 객체 (슬롯): %s, TID: %s"), *GetName(), *m_pWeaponStat->m_strTid);
        UE_LOG(LogTemp, Log, TEXT("드랍하는 객체 (슬롯): %s, TID: %s"), *DragDropOperation->itemSlot->GetName(), *DragDropOperation->m_pWeaponStat->m_strTid);

        if (this == DragDropOperation->itemSlot)
        {
            UE_LOG(LogTemp, Warning, TEXT("강화 실패: 자기 자신과 같은 객체입니다."));
            return false;
        }
        
        if (m_pWeaponStat->m_strTid == DragDropOperation->m_pWeaponStat->m_strTid && m_pWeaponStat->m_strSpriteName == DragDropOperation->m_pWeaponStat->m_strSpriteName)
        {
            if(WeaponUpgrade())
            {
                if(DragDropOperation->m_eBackPackType == nBackPackType::BackPack)
                {
                    if (OnUpgradeCostChanged.IsBound())
                    {
                        UE_LOG(LogTemp, Warning, TEXT("OnUpgradeCostChanged 델리게이트가 바인딩되었습니다."));
                        OnUpgradeCostChanged.Broadcast(m_pWeaponStat->m_fCost);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("OnUpgradeCostChanged 델리게이트가 바인딩되지 않았습니다."));
                    }
                }

                if(g_SoundMgrValid)
                {
                    g_SoundMgr->ButtonPlaySound(0.0f,"Sound_WeaponUpgrade_Cue");
                }
                
                if(Niagara_Upgrade)
                {
                    Niagara_Upgrade->ActivateSystem(true);
                }
                if (UCSUniformGridPanel* ParentGrid = Cast<UCSUniformGridPanel>(DragDropOperation->itemSlot->GetParent()))
                {
                    ParentGrid->RemoveChild(DragDropOperation->itemSlot);
                    ReorderGrid(ParentGrid, 5);
                }
                DragDropOperation->ClearImage();


            }
            else
            {
                DragDropOperation->testImage();
            }
        }
        else
        {
            DragDropOperation->testImage();
            g_GameMsgMgr->ShowMessageBox("Popup_Not_Upgrade");
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("드랍 실패: 드랍하는 객체 또는 드랍 대상 객체가 유효하지 않음."));
    }

    return true;
}


bool UPSWidgetSlot_BackPackItem::WeaponUpgrade()
{
    float UpgradeValue = g_CSDefineRecordMgr->GetValue_Number("GV_WEAPON_UPGRADE_VALUE");

    if (m_pWeaponStat)
    {
        FString strSpriteUpgrade = m_pWeaponStat->m_strSpriteName;
        UpgradeImage(strSpriteUpgrade);

        if (!CSUIUtils::LoadSprite(strSpriteUpgrade))
        {
            g_GameMsgMgr->ShowMessageBox("Popup_Not_LastUpgrade");
            UE_LOG(LogTemp, Warning, TEXT("강화 실패: 유효하지 않은 스프라이트 이름 %s"), *strSpriteUpgrade);
            return false;
        }
        if(m_pWeaponStat)
        {
            m_pWeaponStat->UpgradeLevel = Number;
        }
        switch (m_pWeaponStat->m_eTargetType)
        {
        case nTargetType::Near:
            UE_LOG(LogTemp, Log, TEXT("강화 전 데미지: %f"), m_pWeaponStat->GetLevelDamage());
            m_pWeaponStat->DamageWeaponUpgrade(UpgradeValue);
            UE_LOG(LogTemp, Log, TEXT("강화 후 데미지: %f"), m_pWeaponStat->GetTotalDamage());
            break;
            
        case nTargetType::Random:
            UE_LOG(LogTemp, Log, TEXT("강화 전 데미지: %f"), m_pWeaponStat->GetLevelDamage());
            m_pWeaponStat->DamageWeaponUpgrade(UpgradeValue);
            UE_LOG(LogTemp, Log, TEXT("강화 후 데미지: %f"), m_pWeaponStat->GetTotalDamage());
            break;

        case nTargetType::Armor:
            UE_LOG(LogTemp, Log, TEXT("강화 전 방어력: %f"), m_pWeaponStat->m_fArmor);
            m_pWeaponStat->ArmorWeaponUpgrade(UpgradeValue);
            m_pWeaponStat->HPWeaponUpgrade(UpgradeValue);
            UE_LOG(LogTemp, Log, TEXT("강화 후 방어력: %f"), m_pWeaponStat->m_fArmor);
            break;

        case nTargetType::HP:
            UE_LOG(LogTemp, Log, TEXT("강화 전 체력 회복량: %f"), m_pWeaponStat->m_fHeal);
            UE_LOG(LogTemp, Log, TEXT("강화 전 최대 체력량: %f"), m_pWeaponStat->m_fHP + (m_pWeaponStat->m_fLevelDamage * (m_pWeaponStat->Level-1)));
            m_pWeaponStat->HPWeaponUpgrade(UpgradeValue);
            m_pWeaponStat->HealWeaponUpgrade(UpgradeValue);
            UE_LOG(LogTemp, Log, TEXT("강화 후 체력 회복량: %f"), m_pWeaponStat->m_fHeal);
            UE_LOG(LogTemp, Log, TEXT("강화 전 최대 체력량: %f"), m_pWeaponStat->m_fHP);
            break;

        default:
            UE_LOG(LogTemp, Warning, TEXT("알 수 없는 대상 유형: 강화하지 않음"));
            break;
        }
        m_pWeaponStat->m_strSpriteName = strSpriteUpgrade;
        SetItemImage(m_pWeaponStat);
        
        if (SlotButton_Image_Icon)
        {
            SlotButton_Image_Icon->SetBrushFromSpriteName(m_pWeaponStat->m_strSpriteName);
            return true;
        }
    }
    
    return false;
}

void UPSWidgetSlot_BackPackItem::UpgradeImage(FString& InOutSpriteName)
{
    FString BaseName;
    FString NumberPart;
  

    if (InOutSpriteName.RemoveFromEnd("_Sprite") &&
        InOutSpriteName.Split("_", &BaseName, &NumberPart))
    {
        if (LexTryParseString(Number, *NumberPart))
        {
            Number++;
            InOutSpriteName = FString::Printf(TEXT("%s_%d_Sprite"), *BaseName, Number);
            UE_LOG(LogTemp, Log, TEXT("업그레이드된 스프라이트 이름: %s"), *InOutSpriteName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("숫자 파싱 실패: %s"), *NumberPart);
            InOutSpriteName.Empty();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sprite 이름 형식이 맞지 않음: %s"), *InOutSpriteName);
        InOutSpriteName.Empty();
    }
}



void UPSWidgetSlot_BackPackItem::ReorderGrid(UCSUniformGridPanel* GridPanel, int32 MaxColumns)
{
    if (!GridPanel)
        return;

    TArray<UWidget*> RemainingWidgets;

    for (int32 i = 0; i < GridPanel->GetChildrenCount(); ++i)
    {
        UWidget* ChildWidget = GridPanel->GetChildAt(i);
        if (ChildWidget)
        {
            RemainingWidgets.Add(ChildWidget);
        }
    }

    for (int32 i = 0; i < RemainingWidgets.Num(); ++i)
    {
        UWidget* ChildWidget = RemainingWidgets[i];
        const int32 Row = i / MaxColumns;
        const int32 Column = i % MaxColumns;

        if (ChildWidget->Slot)
        {
            if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ChildWidget->Slot))
            {
                GridSlot->SetRow(Row);
                GridSlot->SetColumn(Column);
            }
        }
    }
    GridPanel->InvalidateLayoutAndVolatility();
}

void UPSWidgetSlot_BackPackItem::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
                                                       UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    if(m_pWeaponStat)
    {
        if(m_pWeaponStat->m_eTargetType == nTargetType::Cost)
        {
            if(g_PSBackPackMgrValid)
            {
                g_PSBackPackMgr->OnCostDrag(false);
            }
        }
    }
    UPSWidget_DragDrop* DragDropOperation = Cast<UPSWidget_DragDrop>(InOperation);
    DragDropOperation->testImage();
}

FReply UPSWidgetSlot_BackPackItem::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        FVector2D CurrentPosition = InMouseEvent.GetScreenSpacePosition();
        float Distance = FVector2D::Distance(DragStartPosition, CurrentPosition);

        if (!bIsDragging && Distance > DragThreshold)
        {
            bIsDragging = true;

            HoldStartTime = -1.0f;
            bIsHolding = false;
            
            StartDragDrop(InGeometry, InMouseEvent);
        }
    }

    return Reply;
}

FReply UPSWidgetSlot_BackPackItem::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        HoldStartTime = -1.0f;
        bIsHolding = false;
        
        if (!bIsDragging)
        {
            if(g_UIMgrValid)
            {
                if(GetWeaponStat())
                {
                    if(GetWeaponStat()->m_eTargetType == nTargetType::Cost)     return Reply.Unhandled();
                    g_PSCharacterMgr->m_pWeaponStat = GetWeaponStat();
                    g_UIMgr->ShowUIPanel(nUIPanelType::WeaponInfo);
                    if(g_SoundMgrValid)
                    {
                        g_SoundMgr->ButtonPlaySound(0.0f,"Sound_Click_Cue");
                    }
                }
            }
        }
    }

    return Reply.Unhandled();
}

void UPSWidgetSlot_BackPackItem::StartDragDrop(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UDragDropOperation* DragDropOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

    if (DragDropOperation)
    {
        DragDropOperation->Payload = this;
        DragDropOperation->DefaultDragVisual = this;
        DragDropOperation->Pivot = EDragPivot::MouseDown;



        
        FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    }
}



