// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreLibrary/Manager/CSBaseManager.h"
#include "PSPurchaseMgr.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FPurchaseAdRemoveComplete,int,amount,const FString&,type);
DECLARE_MULTICAST_DELEGATE(FPurchasedAdRemove);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FPurchaseDiaComplete,int,amount,const FString&,type);

UCLASS()
class PROTOSURVIVOR_API UPSPurchaseMgr : public UCSBaseManager
{
	GENERATED_BODY()
	
public:
	static FPurchaseAdRemoveComplete PurchaseAdRemoveComplete;
	FPurchasedAdRemove PurchasedAdRemove;

	static FPurchaseDiaComplete PurchaseDiaComplete;

	static FString strType;
	static int Amount;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Load() override;
	virtual void Release() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Purchase_AdRemove(int32 amount, const FString& type);
	UFUNCTION()
	void PurchaseResult_AdRemove(int32 amount, const FString& type);

	UFUNCTION(BlueprintCallable)
	void Purchase_Dia(int32 amount, const FString& type);
	UFUNCTION()
	void PurchaseResult_Dia(int32 amount, const FString& type);
	
#if PLATFORM_ANDROID
	static void onNativePruchaseResult(JNIEnv* env, jobject obj, jint amount , jstring type);
#endif
};
#define g_PSPurchaseMgr (g_GameGlobal->GetManager<UPSPurchaseMgr>())
#define g_PSPurchaseMgrValid (g_GameGlobal->IsValidManager<UPSPurchaseMgr>())