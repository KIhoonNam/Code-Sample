// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreLibrary/Manager/CSBaseManager.h"
#include "PSAdMobMgr.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FRespawnRewardComplete,int,amount,const FString&,type);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBackpackRefreshRewardComplete,int,amount,const FString&,type);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FPropertyRefreshRewardComplete,int,amount,const FString&,type);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FPropertyAllRewardComplete,int,amount,const FString&,type);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FStageClearRewardComplete,int,amount,const FString&,type);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FBackpackSilverRewardComplete,int,amount,const FString&,type);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FCombatSpeedRewardComplete,int,amount,const FString&,type);

class UPSWidget_CombatSpeed;
class UPSWidget_WaveBackPack;
class UPSWidget_Properties;

UCLASS()
class PROTOSURVIVOR_API UPSAdMobMgr : public UCSBaseManager
{
	GENERATED_BODY()
	
public:
	static FRespawnRewardComplete RespawnRewardComplete;
	static FBackpackRefreshRewardComplete BackpackRefreshRewardComplete;
	static FPropertyRefreshRewardComplete PropertyRefreshRewardComplete;
	static FPropertyAllRewardComplete PropertyAllRewardComplete;
	static FStageClearRewardComplete StageClearRewardComplete;
	static FBackpackSilverRewardComplete BackpackSilverRewardComplete;
	static FCombatSpeedRewardComplete CombatSpeedRewardComplete;

	static FString strType;
	static int Amount;

	UPSWidget_Properties* PropertyWidget;
	UPSWidget_WaveBackPack* BackpackWidget;
	UPSWidget_StageClear* StageClearWidget;
	UPSWidget_CombatSpeed* CombatSpeedWidget;
		
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Load() override;
	virtual void Release() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LoadRewardAd(const FString& _LoadRewardId);

	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_Respawn(int32 amount, const FString& type);
	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_PropertyRefresh(int32 amount, const FString& type, UPSWidget_Properties* properties);
	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_PropertyAll(int32 amount, const FString& type, UPSWidget_Properties* properties);
	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_BackpackRefresh(int32 amount, const FString& type, UPSWidget_WaveBackPack* backpack);
	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_StageClear(int32 amount, const FString& type, UPSWidget_StageClear* stageClear);
	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_BackpackSilver(int32 amount, const FString& type, UPSWidget_WaveBackPack* backpack);
	UFUNCTION(BlueprintCallable)
	void ShowRewardAd_CombatSpeed(int32 amount, const FString& type, UPSWidget_CombatSpeed* combatSpeed);
	
	UFUNCTION()
	void RewardResult_Respawn(int32 amount,const FString& type);
	UFUNCTION()
	void RewardResult_PropertyRefresh(int32 amount,const FString& type);
	UFUNCTION()
	void RewardResult_PropertyAll(int32 amount,const FString& type);
	UFUNCTION()
	void RewardResult_BackpackRefresh(int32 amount, const FString& type);
	UFUNCTION()
	void RewardResult_StageClear(int32 amount, const FString& type);
	UFUNCTION()
	void RewardResult_BackpackSilver(int32 amount, const FString& type);
	UFUNCTION()
	void RewardResult_CombatSpeed(int32 amount, const FString& type);
	
#if PLATFORM_ANDROID
	static void onNativeRewardResult(JNIEnv* env, jobject obj, jint amount , jstring type);
	static void onNativeLoadFailed(JNIEnv* env, jobject obj, jstring type);
#endif
};

#define g_PSAdMobMgr (g_GameGlobal->GetManager<UPSAdMobMgr>())
#define g_PSAdMobMgrValid (g_GameGlobal->IsValidManager<UPSAdMobMgr>())