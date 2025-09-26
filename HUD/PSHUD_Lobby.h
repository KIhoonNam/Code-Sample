// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/HUD/CSHUD.h"
#include "PSHUD_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API APSHUD_Lobby : public ACSHUD
{
	GENERATED_BODY()

	virtual void CreateWidgetPanels() override;
};


