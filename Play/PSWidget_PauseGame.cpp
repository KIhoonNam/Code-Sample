// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidget_PauseGame.h"

#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "UILibrary/Widget/BaseWidget/CSButton.h"
#include "ProtoSurvivor/UI/Play/PSWidget_Sound.h"

void UPSWidget_PauseGame::InitData()
{
	Super::InitData();

	if (SoundWidget)
	{
		SoundWidget->Hide();
	}
}

void UPSWidget_PauseGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPSWidget_PauseGame::LinkEvent()
{
	Super::LinkEvent();

	BIND_BUTTON_EVENT(Button_Cancel, &UPSWidget_PauseGame::OnClicked_Cancel);
	BIND_BUTTON_EVENT(Button_Lobby, &UPSWidget_PauseGame::OnClicked_Lobby);
	BIND_BUTTON_EVENT(Button_Sound, &UPSWidget_PauseGame::OnClicked_Sound);
}

void UPSWidget_PauseGame::UnLinkEvent()
{
	UNBIND_BUTTON_EVENT_ALL(Button_Cancel);
	UNBIND_BUTTON_EVENT_ALL(Button_Lobby);
	UNBIND_BUTTON_EVENT_ALL(Button_Sound);
	
	Super::UnLinkEvent();
}

void UPSWidget_PauseGame::Show(ESlateVisibility visible, float fDelayTime, float fFadeTime, float fDisplayTime)
{
	Super::Show(visible, fDelayTime, fFadeTime, fDisplayTime);

	if (Animation_Open)
	{
		PlayAnimation(Animation_Open);
	}
}

void UPSWidget_PauseGame::OnClicked_Cancel()
{
	Hide();
	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::PauseMenu);
	}
}

void UPSWidget_PauseGame::OnClicked_Lobby()
{
	if (g_PSPauseMgrValid)
	{
		g_PSPauseMgr->ClearAllPauseReasons(GetWorld());
	}
	UGameplayStatics::OpenLevel(GetWorld(),"Level_Lobby");
}

void UPSWidget_PauseGame::OnClicked_Sound()
{
	if (SoundWidget)
	{
		SoundWidget->Show();
	}
}
