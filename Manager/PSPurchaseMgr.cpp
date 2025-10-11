// Fill out your copyright notice in the Description page of Project Settings.


#include "PSPurchaseMgr.h"

#include "PSCharacterMgr.h"
#include "PSDataBaseMgr.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <jni.h>
#endif

FPurchaseAdRemoveComplete UPSPurchaseMgr::PurchaseAdRemoveComplete;
FPurchaseDiaComplete UPSPurchaseMgr::PurchaseDiaComplete;
int UPSPurchaseMgr::Amount;
FString UPSPurchaseMgr::strType;

void UPSPurchaseMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPSPurchaseMgr::Deinitialize()
{
	UPSPurchaseMgr::PurchaseAdRemoveComplete.Unbind();
	UPSPurchaseMgr::PurchaseDiaComplete.Unbind();
	
	Super::Deinitialize();
}

void UPSPurchaseMgr::Load()
{
	Super::Load();
}

void UPSPurchaseMgr::Release()
{
	Super::Release();
}

void UPSPurchaseMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UPSPurchaseMgr::Purchase_AdRemove(int32 amount, const FString& type)
{
	FString ProductTo = type;

	FPurchaseAdRemoveComplete PurchaseComplete_AdRemove;
	PurchaseComplete_AdRemove.BindDynamic(this,&UPSPurchaseMgr::PurchaseResult_AdRemove);
	UPSPurchaseMgr::PurchaseAdRemoveComplete.operator=(PurchaseComplete_AdRemove);
	
#if WITH_EDITOR
	if(UPSPurchaseMgr::PurchaseAdRemoveComplete.IsBound())
	{
		UPSPurchaseMgr::PurchaseAdRemoveComplete.Execute(amount, type);
	}
#endif
	
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto ProductID = FJavaHelper::ToJavaString(Env,ProductTo);
		static jmethodID ParchaseResult = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_LaunchPurchaseFlow", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ParchaseResult, *ProductID);
	}
#endif
}

void UPSPurchaseMgr::PurchaseResult_AdRemove(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (amount == 0)
		{
			if (g_PSCharacterMgrValid)
			{
				g_PSCharacterMgr->SetPurchasedAdRemove(true);
			}
            		
			if (PurchasedAdRemove.IsBound())
			{
				PurchasedAdRemove.Broadcast();
			}
		}
	});
}

void UPSPurchaseMgr::Purchase_Dia(int32 amount, const FString& type)
{
	FString ProductTo = type;
	
	FPurchaseDiaComplete PurchaseComplete_Dia;
	PurchaseComplete_Dia.BindDynamic(this, &UPSPurchaseMgr::PurchaseResult_Dia);
	UPSPurchaseMgr::PurchaseDiaComplete.operator=(PurchaseComplete_Dia);
	
#if WITH_EDITOR
	if(UPSPurchaseMgr::PurchaseDiaComplete.IsBound())
	{
		UPSPurchaseMgr::PurchaseDiaComplete.Execute(amount, type);
	}
#endif
	
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto ProductID = FJavaHelper::ToJavaString(Env,ProductTo);
		static jmethodID ParchaseResult = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_LaunchPurchaseFlow", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ParchaseResult, *ProductID);
	}
#endif
}

void UPSPurchaseMgr::PurchaseResult_Dia(int32 amount, const FString& type)
{
	AsyncTask(ENamedThreads::GameThread,[=, this]
	{
		if (amount == 0)
		{	
			if (g_PSCharacterMgrValid)
			{
				g_PSCharacterMgr->SetDia(g_PSCharacterMgr->GetDia() + g_PSCharacterMgr->m_SelectedGoodsNumber);
			}
		}
	});
}

#if PLATFORM_ANDROID
extern "C" JNIEXPORT void JNICALL
Java_com_epicgames_unreal_GameActivity_onNativePruchaseResult(JNIEnv* env, jobject obj, jint amount , jstring type)
{
	const char* nativeString = env->GetStringUTFChars(type, 0);

	UPSPurchaseMgr::Amount = amount;
	
	// 콜백 처리
	AsyncTask(ENamedThreads::GameThread,[&]
	{
		if(UPSPurchaseMgr::PurchaseAdRemoveComplete.IsBound())
		{
			UPSPurchaseMgr::PurchaseAdRemoveComplete.Execute(UPSPurchaseMgr::Amount,UPSPurchaseMgr::strType);
		}
		else if (UPSPurchaseMgr::PurchaseDiaComplete.IsBound())
		{
			UPSPurchaseMgr::PurchaseDiaComplete.Execute(UPSPurchaseMgr::Amount, UPSPurchaseMgr::strType);
		}
	});

	// JNI 메모리 해제
	env->ReleaseStringUTFChars(type, nativeString);
}
#endif
