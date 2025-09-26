// Fill out your copyright notice in the Description page of Project Settings.


#include "PSWidgetPanel_Splash.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"

void UPSWidgetPanel_Splash::InitData()
{
	Super::InitData();

	if(Anim_LOGO)
	{
		PlayAnimation(Anim_LOGO);
	}
}

void UPSWidgetPanel_Splash::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_IsStart)
	{
		m_fTime += InDeltaTime;

		if(m_fTime >= 2.f)
		{
#if WITH_EDITOR
			g_DataBaseMgr->PCTestDatabase();
#else
			//g_DataBaseMgr->PCTestDatabase();
			g_DataBaseMgr->GooglePlayLogin();
#endif
			m_IsStart = false;
			m_fTime = 0.f;
		}
	}
}

void UPSWidgetPanel_Splash::LinkEvent()
{
	Super::LinkEvent();

	if(Anim_LOGO)
	{
		OnLogoFinish.BindDynamic(this,&UPSWidgetPanel_Splash::OnFinishLogo);
		BindToAnimationFinished(Anim_LOGO,OnLogoFinish);
	}

	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this,&UPSWidgetPanel_Splash::OnLoadDatabase);
	}
}

void UPSWidgetPanel_Splash::UnLinkEvent()
{
	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
	}
	if(Anim_LOGO)
	{
		UnbindAllFromAnimationFinished(Anim_LOGO);
	}
	Super::UnLinkEvent();
}

void UPSWidgetPanel_Splash::OnFinishLogo()
{
	if(g_DataBaseMgrValid)
	{

		m_IsStart = true;
		m_fTime = 0.f;
	}
}

void UPSWidgetPanel_Splash::OnLoadDatabase()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName("Level_Lobby"));
}
