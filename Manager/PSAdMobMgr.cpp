// Fill out your copyright notice in the Description page of Project Settings.


#include "PSAdMobMgr.h"

#include <string>

#include "PSCharacterMgr.h"
#include "PSPauseMgr.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/UI/Play/PSWidget_CombatSpeed.h"
#include "ProtoSurvivor/UI/Play/PSWidget_Properties.h"
#include "ProtoSurvivor/UI/Play/PSWidget_WaveBackPack.h"
#include "ProtoSurvivor/UI/Play/PSWidget_StageClear.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <jni.h>
#endif

FRespawnRewardComplete UPSAdMobMgr::RespawnRewardComplete;
FBackpackRefreshRewardComplete UPSAdMobMgr::BackpackRefreshRewardComplete;
FPropertyRefreshRewardComplete UPSAdMobMgr::PropertyRefreshRewardComplete;
FPropertyAllRewardComplete UPSAdMobMgr::PropertyAllRewardComplete;
FStageClearRewardComplete UPSAdMobMgr::StageClearRewardComplete;
FBackpackSilverRewardComplete UPSAdMobMgr::BackpackSilverRewardComplete;
FCombatSpeedRewardComplete UPSAdMobMgr::CombatSpeedRewardComplete;
int UPSAdMobMgr::Amount;
FString UPSAdMobMgr::strType;

void UPSAdMobMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPSAdMobMgr::Deinitialize()
{
	UPSAdMobMgr::RespawnRewardComplete.Unbind();
	UPSAdMobMgr::BackpackRefreshRewardComplete.Unbind();
	UPSAdMobMgr::PropertyRefreshRewardComplete.Unbind();
	UPSAdMobMgr::PropertyAllRewardComplete.Unbind();
	UPSAdMobMgr::BackpackSilverRewardComplete.Unbind();
	UPSAdMobMgr::CombatSpeedRewardComplete.Unbind();
	
	Super::Deinitialize();
}

void UPSAdMobMgr::Load()
{
	Super::Load();
	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

void UPSAdMobMgr::Release()
{
	Super::Release();
}

void UPSAdMobMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UPSAdMobMgr::LoadRewardAd(const FString& _LoadRewardId)
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto RewardId = FJavaHelper::ToJavaString(Env,_LoadRewardId);
		static jmethodID LoadReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_LoadRewardAd", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, LoadReward, *RewardId);
	}
#endif
}

void UPSAdMobMgr::ShowRewardAd_Respawn(int32 amount, const FString& type)
{
	FRespawnRewardComplete RespawnRewared;
	RespawnRewared.BindDynamic(this,&UPSAdMobMgr::RewardResult_Respawn);
	UPSAdMobMgr::RespawnRewardComplete.operator=(RespawnRewared);

	if (g_PSCharacterMgrValid)
	{
		if (g_PSCharacterMgr->GetPurchasedAdRemove())
		{
			if(UPSAdMobMgr::RespawnRewardComplete.IsBound())
			{
				UPSAdMobMgr::RespawnRewardComplete.Execute(amount, type);
				return;
			}
		}
	}
	
#if WITH_EDITOR
	if(UPSAdMobMgr::RespawnRewardComplete.IsBound())
	{
		UPSAdMobMgr::RespawnRewardComplete.Execute(amount, type);
	}
#endif
	
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
	}
#endif
}

void UPSAdMobMgr::ShowRewardAd_PropertyRefresh(int32 amount, const FString& type, UPSWidget_Properties* properties)
{
	if (properties)
	{
		PropertyWidget = properties;
		FPropertyRefreshRewardComplete PropertyRefreshReward;
		PropertyRefreshReward.BindDynamic(this,&UPSAdMobMgr::RewardResult_PropertyRefresh);
		UPSAdMobMgr::PropertyRefreshRewardComplete.operator=(PropertyRefreshReward);

		if (g_PSCharacterMgrValid)
		{
			if (g_PSCharacterMgr->GetPurchasedAdRemove())
			{
				if(UPSAdMobMgr::PropertyRefreshRewardComplete.IsBound())
				{
					UPSAdMobMgr::PropertyRefreshRewardComplete.Execute(amount, type);
					return;
				}
			}
		}
		
#if WITH_EDITOR
		if(UPSAdMobMgr::PropertyRefreshRewardComplete.IsBound())
		{
			UPSAdMobMgr::PropertyRefreshRewardComplete.Execute(amount, type);
		}
#endif
	
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
#endif
	}
}

void UPSAdMobMgr::ShowRewardAd_PropertyAll(int32 amount, const FString& type, UPSWidget_Properties* properties)
{
	if (properties)
	{
		PropertyWidget = properties;
		FPropertyAllRewardComplete PropertyAllReward;
		PropertyAllReward.BindDynamic(this,&UPSAdMobMgr::RewardResult_PropertyAll);
		UPSAdMobMgr::PropertyAllRewardComplete.operator=(PropertyAllReward);

		if (g_PSCharacterMgrValid)
		{
			if (g_PSCharacterMgr->GetPurchasedAdRemove())
			{
				if(UPSAdMobMgr::PropertyAllRewardComplete.IsBound())
				{
					UPSAdMobMgr::PropertyAllRewardComplete.Execute(amount, type);
					return;
				}
			}
		}
		
#if WITH_EDITOR
		if(UPSAdMobMgr::PropertyAllRewardComplete.IsBound())
		{
			UPSAdMobMgr::PropertyAllRewardComplete.Execute(amount, type);
		}
#endif
	
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
#endif
	}
}

void UPSAdMobMgr::ShowRewardAd_BackpackRefresh(int32 amount, const FString& type, UPSWidget_WaveBackPack* backpack)
{
	if (backpack)
	{
		BackpackWidget = backpack;
		FBackpackRefreshRewardComplete BackpackRefreshReward;
		BackpackRefreshReward.BindDynamic(this,&UPSAdMobMgr::RewardResult_BackpackRefresh);
		UPSAdMobMgr::BackpackRefreshRewardComplete.operator=(BackpackRefreshReward);

		if (g_PSCharacterMgrValid)
		{
			if (g_PSCharacterMgr->GetPurchasedAdRemove())
			{
				if(UPSAdMobMgr::BackpackRefreshRewardComplete.IsBound())
				{
					UPSAdMobMgr::BackpackRefreshRewardComplete.Execute(amount, type);
					return;
				}
			}
		}
		
#if WITH_EDITOR
		if(UPSAdMobMgr::BackpackRefreshRewardComplete.IsBound())
		{
			UPSAdMobMgr::BackpackRefreshRewardComplete.Execute(amount, type);
		}
#endif
	
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
#endif
	}
}

void UPSAdMobMgr::ShowRewardAd_StageClear(int32 amount, const FString& type, UPSWidget_StageClear* stageClear)
{
	if (stageClear)
	{
		StageClearWidget = stageClear;
		FStageClearRewardComplete StageClearReward;
		StageClearReward.BindDynamic(this,&UPSAdMobMgr::RewardResult_StageClear);
		UPSAdMobMgr::StageClearRewardComplete.operator=(StageClearReward);

		if (g_PSCharacterMgrValid)
		{
			if (g_PSCharacterMgr->GetPurchasedAdRemove())
			{
				if(UPSAdMobMgr::StageClearRewardComplete.IsBound())
				{
					UPSAdMobMgr::StageClearRewardComplete.Execute(amount, type);
					return;
				}
			}
		}
		
#if WITH_EDITOR
		if(UPSAdMobMgr::StageClearRewardComplete.IsBound())
		{
			UPSAdMobMgr::StageClearRewardComplete.Execute(amount, type);
		}
#endif
	
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
#endif
	}
}

void UPSAdMobMgr::ShowRewardAd_BackpackSilver(int32 amount, const FString& type, UPSWidget_WaveBackPack* backpack)
{
	if (backpack)
	{
		BackpackWidget = backpack;
		FBackpackSilverRewardComplete BackpackSilverReward;
		BackpackSilverReward.BindDynamic(this,&UPSAdMobMgr::RewardResult_BackpackSilver);
		UPSAdMobMgr::BackpackSilverRewardComplete.operator=(BackpackSilverReward);

		if (g_PSCharacterMgrValid)
		{
			if (g_PSCharacterMgr->GetPurchasedAdRemove())
			{
				if(UPSAdMobMgr::BackpackSilverRewardComplete.IsBound())
				{
					UPSAdMobMgr::BackpackSilverRewardComplete.Execute(amount, type);
					return;
				}
			}
		}
		
#if WITH_EDITOR
		if(UPSAdMobMgr::BackpackSilverRewardComplete.IsBound())
		{
			UPSAdMobMgr::BackpackSilverRewardComplete.Execute(amount, type);
		}
#endif
	
#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
#endif
	}
}

void UPSAdMobMgr::ShowRewardAd_CombatSpeed(int32 amount, const FString& type, UPSWidget_CombatSpeed* combatSpeed)
{
	if (combatSpeed)
	{
		CombatSpeedWidget = combatSpeed;
		FCombatSpeedRewardComplete CombatSpeedReward;
		CombatSpeedReward.BindDynamic(this,&UPSAdMobMgr::RewardResult_CombatSpeed);
		UPSAdMobMgr::CombatSpeedRewardComplete.operator=(CombatSpeedReward);

	#if WITH_EDITOR
		if(UPSAdMobMgr::CombatSpeedRewardComplete.IsBound())
		{
			UPSAdMobMgr::CombatSpeedRewardComplete.Execute(amount, type);
		}
	#endif
		
	#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			static jmethodID ShowReward = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowRewardAd", "()V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowReward);
		}
	#endif
	}
}

void UPSAdMobMgr::RewardResult_BackpackSilver(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (BackpackWidget)
		{
			BackpackWidget->GetSilver();
		}
	});
	
	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

void UPSAdMobMgr::RewardResult_CombatSpeed(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (CombatSpeedWidget)
		{
			CombatSpeedWidget->SetCanChangeCombatSpeed();
		}	
	});
	
	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

void UPSAdMobMgr::RewardResult_StageClear(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread, [=, this]
	{
		if (StageClearWidget)
		{
			if (g_PSPauseMgrValid)
			{
				g_PSPauseMgr->ClearAllPauseReasons(GetWorld());
			}
			
			StageClearWidget->GetRewardBonus();
			UGameplayStatics::OpenLevel(GetWorld(),"Level_Lobby");
		}
	});

	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

void UPSAdMobMgr::RewardResult_Respawn(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (APSCharacterBase* playerCharacter =
			Cast<APSCharacterBase>(UGameplayStatics::GetPlayerCharacter(g_GameGlobal->GetCurWorld(), 0)))
		{
			if (g_UIMgrValid)
			{
				playerCharacter->SetRespawn(true);
				playerCharacter->SetCurrentHP_Heal(playerCharacter->GetMaxHP());
				g_UIMgr->HideUIPanel(nUIPanelType::Respawn);
				if(g_PSPauseMgrValid)
				{
					g_PSPauseMgr->RemovePauseReason(GetWorld(), nPauseType::ReSpawn);
				}
			}
		}
	});
	
	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

void UPSAdMobMgr::RewardResult_PropertyRefresh(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (PropertyWidget)
		{
			PropertyWidget->Refresh();
		}
	});

	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

void UPSAdMobMgr::RewardResult_PropertyAll(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (PropertyWidget)
		{
			PropertyWidget->GetAllProperties();
		}
	});
	
	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");	
}

void UPSAdMobMgr::RewardResult_BackpackRefresh(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (BackpackWidget)
		{
			BackpackWidget->Refresh_Item();
		}
	});
	
	LoadRewardAd("ca-app-pub-5747602554912250/6663256516");
}

#if PLATFORM_ANDROID
extern "C" JNIEXPORT void JNICALL
Java_com_epicgames_unreal_GameActivity_onNativeRewardResult(JNIEnv* env, jobject obj, jint amount , jstring type)
{
	const char* nativeString = env->GetStringUTFChars(type, 0);
	UPSAdMobMgr::strType = UTF8_TO_TCHAR(nativeString);
	UPSAdMobMgr::Amount = amount;
	UE_LOG(LogTemp, Log, TEXT("Received result: %s"), *UPSAdMobMgr::strType);
	
	// 콜백 처리
	AsyncTask(ENamedThreads::GameThread,[&]
	{
		if(UPSAdMobMgr::Amount == -1)
		{
			g_GameMsgMgr->ShowMessageBox("Popup_Not_Ad");
		}
		else
		{
			if(UPSAdMobMgr::RespawnRewardComplete.IsBound())
			{
				UPSAdMobMgr::RespawnRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::RespawnRewardComplete.Unbind();
			}
			else if (UPSAdMobMgr::BackpackRefreshRewardComplete.IsBound())
			{
				UPSAdMobMgr::BackpackRefreshRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::BackpackRefreshRewardComplete.Unbind();
			}
			else if (UPSAdMobMgr::PropertyRefreshRewardComplete.IsBound())
			{
				UPSAdMobMgr::PropertyRefreshRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::PropertyRefreshRewardComplete.Unbind();
			}
			else if (UPSAdMobMgr::PropertyAllRewardComplete.IsBound())
			{
				UPSAdMobMgr::PropertyAllRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::PropertyAllRewardComplete.Unbind();
			}
			else if (UPSAdMobMgr::StageClearRewardComplete.IsBound())
			{
				UPSAdMobMgr::StageClearRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::StageClearRewardComplete.Unbind();
			}
			else if (UPSAdMobMgr::BackpackSilverRewardComplete.IsBound())
			{
				UPSAdMobMgr::BackpackSilverRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::BackpackSilverRewardComplete.Unbind();
			}
			else if (UPSAdMobMgr::CombatSpeedRewardComplete.IsBound())
			{
				UPSAdMobMgr::CombatSpeedRewardComplete.Execute(UPSAdMobMgr::Amount,UPSAdMobMgr::strType);
				UPSAdMobMgr::CombatSpeedRewardComplete.Unbind();
			}
		}
	});

	// JNI 메모리 해제
	env->ReleaseStringUTFChars(type, nativeString);
}
#endif