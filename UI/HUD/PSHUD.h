// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/HUD/CSHUD.h"
#include "PSHUD.generated.h"

UCLASS()
class PROTOSURVIVOR_API APSHUD : public ACSHUD
{
	GENERATED_BODY()

	virtual void CreateWidgetPanels() override;
};
