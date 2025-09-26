// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_Stage.generated.h"

UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_Stage : public UCSWidgetSlot
{
	GENERATED_BODY()

public:
	virtual void InitData() override;
	virtual void ReleaseData() override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;
};
