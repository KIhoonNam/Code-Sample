// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Auth/Auth.h"
#include "CoreLibrary/Manager/CSBaseManager.h"
#include "Database/Database.h"
#include "Database/DatabaseReference.h"
#include "PSDataBaseMgr.generated.h"

class UDatabase;
class UDatabaseReference;
DECLARE_MULTICAST_DELEGATE_ThreeParams(FWeaponLevelup,const FString&,int32,int32);
DECLARE_MULTICAST_DELEGATE(FLoadCompleteDataBase)
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSDataBaseMgr : public UCSBaseManager
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UDatabase* m_pDatabase;

	UPROPERTY()
	FCredential Credential;

	FLoadCompleteDataBase LoadCompleteDataBase;
	FWeaponLevelup WeaponLevelup;
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Load() override;
	virtual void Release() override;
	virtual void Tick(float DeltaTime) override;
	

public:
	UFUNCTION(BlueprintCallable)
	void GooglePlayLogin();

public:
	void GetCharacterStat(UDataSnapshot* dataSnapshot);
	void GetCharacterGoods(UDataSnapshot* dataSnapshot);
	void GetWeaponDetail(UDataSnapshot* DataSnapshot);
	void GetStageInfo(UDataSnapshot* DataSnapshot);
	void SetCharacterStats(const FString& _statName,int32 _level);

	void SetCharacterGoods(const FString& _statName,int32 _level);
	void SetCharacterGoodsString(const FString& _statName, const FString& _value);

	void SetWeaponEquip(const FString& _weaponName,bool _isEquip);
	void SetWeaponLevel(const FString& _weaponName, int32 _level,int32 piece);
	void SetWeaponPiece(const FString& _weaponName,int32 Piece);
	void SetWeaponUnLock(const FString& _weaponName);
	void SetStageInfo(int32 maxStage);
	void SetStageHighWave(int32 _stageNumber,int32 _highWave);
	void InitCharacterStat();
	void InitCharacterGoods();
	void InitWeaponDetail();
	void InitStageInfo();

	void GetPurchaseInfo(UDataSnapshot* dataSnapshot);
	void SetPurchaseInfo(const FString& _statName, bool _purchase);
	void InitPurchaseInfo();

	void InitStoreInfo();
	void GetStoreInfo(UDataSnapshot* dataSnapshot);
	void SetStoreInfo(const FString& storelistName, bool _enable);
	
	void OnOverWeaponLevelup(EFirebaseDatabaseError FirebaseDatabaseError);
public:
	bool GetIsLoad(){return m_IsLoad;}

	void PCTestDatabase();
private:
	void SnapShotActionOver(EFirebaseDatabaseError FirebaseDatabaseError, UDataSnapshot* DataSnapshot);
	void OnActionOver(EFirebaseAuthError FirebaseAuthError, UUser* User);
	void OnTaskOver(bool bArg, FString String);
	void OnOverSetStat(EFirebaseDatabaseError FirebaseDatabaseError);


	void GoogldLoginFailed();
	
	void GoogldLoginFailedTo();

	UFUNCTION()
	void OnFailedConnectFirebase();
private:
	bool bUsePlayGames = true;

	bool m_bGoogleEnable;

	bool m_bGooglePlayLogin;

		FString m_strUserUID;
    	FString m_strUserName;
    	FString m_strPhotoURL;

private:
	bool m_IsLoad = false;



	int32 m_Piece = 0;
	int32 m_Levelup = 0;
	FString m_strWeaponTid;
	
};

#define g_DataBaseMgrValid (g_GameGlobal->IsValidManager<UPSDataBaseMgr>())
#define g_DataBaseMgr (g_GameGlobal->GetManager<UPSDataBaseMgr>())