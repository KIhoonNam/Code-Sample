// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "Blueprint/DragDropOperation.h"
#include "ProtoSurvivor/Define/PSCommon_BackPack.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "PSWidget_DragDrop.generated.h"

class UPSWidgetSlot_BackPackItem;
class UTexture2D;
class UPaperSprite;
struct FWeaponStat;
UCLASS()
class PROTOSURVIVOR_API UPSWidget_DragDrop : public UDragDropOperation
{
	GENERATED_BODY()

public:
	FWeaponStat* m_pWeaponStat;
	
	// Reference to the dragged widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop")
	UUserWidget* WidgetReference;

	// Offset from where the drag started
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop")
	FVector2D DragOffset;

	UPROPERTY()
	UPaperSprite* itemImage;
	UPROPERTY()
	UPSWidgetSlot_BackPackItem* itemSlot;
	int32 myKey;

	void ClearImage();
	void testImage();

	float GetCost(){ if(m_pWeaponStat) return m_pWeaponStat->m_fCost; return 0; }

	nBackPackType::en m_eBackPackType = nBackPackType::Max;
};
