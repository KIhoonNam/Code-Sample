// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary/Widget/CSWidgetPanel.h"
#include "PSWidgetPanel_Lobby.generated.h"

class UCSButton;
class UCSWidgetSwitcher;
class UPSWidget_WaveBackPack;
class UCSWidgetScrollBox;

UCLASS()
class PROTOSURVIVOR_API UPSWidgetPanel_Lobby : public UCSWidgetPanel
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidgetOptional))
	UCSWidgetSwitcher* WidgetSwitcher_Lobby;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCSWidgetScrollBox* Widget_ScrollboxLobby;
public:
	virtual void InitData() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void LinkEvent() override;
	virtual void UnLinkEvent() override;

	virtual void OnChildItemClicked(UCSUserWidgetBase* _pChildWidget) override;

public:
	void VisibleMenu(int32 _numberMenu);

	void SelectScrollBoxLobbyChild(int32 _numberMenu);
};
