// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetSlot.h"
#include "PSWidgetSlot_LobbyMenu.generated.h"


class UCSImage;
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSWidgetSlot_LobbyMenu : public UCSWidgetSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 MenuNumber;

	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_Icon;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSImage* Image_SelectIcon;

	UPROPERTY(EditAnywhere,DisplayName="Image")
	FSlateBrush m_IconBrush;


public:
	virtual void SynchronizeProperties() override;
};
