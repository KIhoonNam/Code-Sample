// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreLibrary/Manager/CSBaseManager.h"
#include "ProtoSurvivor/Define/PSCommon.h"
#include "PSCharacterMgr.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FChangedGold, int32,nGoodsType::en);
DECLARE_MULTICAST_DELEGATE(FRefreshStat);
DECLARE_MULTICAST_DELEGATE(FRefreshGoods);
DECLARE_MULTICAST_DELEGATE(FRefreshWeapon);
DECLARE_DELEGATE_OneParam(FMoveGoodsStore,int32);
struct FWeaponStat;
class UDataSnapshot;
UCLASS()
class PROTOSURVIVOR_API UPSCharacterMgr : public UCSBaseManager
{
	GENERATED_BODY()

public:	
	FChangedGold ChangedGold;
	FRefreshStat RefreshStat;
	FRefreshGoods RefreshGoods;
	FRefreshWeapon RefreshWeapon;
	FMoveGoodsStore MoveGoodsStore;
	FWeaponStat* m_pWeaponStat;
	UPROPERTY()
	TMap<FString,int32> m_MapCharacterLevel;


	TMap<FString,FWeaponStat*> m_MapWeapon;

	TMap<int32,FWeaponStat*> m_mapEquipWeapon;
public:
	void SetMaxHP(float NewMaxHP);
	float GetMaxHP() const;

	int32 GetLevel(const FString& _name);

	int32 GetDia(){return m_Dia;}
	FString GetstrDia();
	void SetDia(int32 _dia);
	
	int32 GetGold(){return m_Gold;}
	FString GetstrGold();
	void SetGold(int32 _gold);
	
	int32 GetEnergy(){return m_Energy;}
	FString GetstrEnergy();
	void SetEnergy(int32 _energy);
	void StartEnergyTick(float EnergyInterval);
	void StopEnergyTick();

	void SetDisableFreeGold(bool _enable) {m_DisableFreeGold = _enable;}
	bool GetDisableFreeGold(){return m_DisableFreeGold;}
	
	float GetEnergyAccumulator() const { return m_EnergyAccumulator; }

	bool GetPurchasedAdRemove() const {return m_bPurchasedAdRemove;}
	void SetPurchasedAdRemove(bool bPurchasedAdRemove);

	void UnlockWeapons(const FString& _strTid);

	void ChangeEquipWeapon(int32 number,FWeaponStat* _weaponstat,bool _isEquip);
	FCharacterStat* GetCharacterStat(){return m_CharacterStat;}

	UFUNCTION(BlueprintCallable)
	void TestPointer();
public:
	void InitCharacterStat();
	void InitCharacterGoods(UDataSnapshot* _dataSnapShot);
	void InitWeapon();

	void InitPurchaseInfo(UDataSnapshot* _dataSnapShot);

	void OnRefreshWeapon();
	
	float GetStatValue(const FString& _name);
	void SetStatValue(const FString& _name,int32 _level =1);
	
	void SetWeaponPiece(const FString& String, int32 INT32);

	float GetCharacterStatCoolTime();
	float GetCharacterStatArmor();
	float GetCharacterStatMonsterMove();
	float GetCharacterStatMonsterAttack();

	void ProcessLoadedData();
	void SetLastSavedTime(const FString& SavedTime);

	void OnMoveGoodsStore(int32 menuNumber);
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Load() override;
	virtual void Release() override;
	virtual void Tick(float DeltaTime) override;

public:
	int32 m_SelectedGoodsPrice;
	int32 m_SelectedGoodsNumber;
	
private:
	FCharacterStat* m_CharacterStat;

	bool m_DisableFreeGold = false;
	
	int32 m_Gold;
	int32 m_Dia;
	int32 m_Energy = 0;
	
	int32 m_MaxEnergy;
	bool m_bPurchasedAdRemove;
	float m_EnergyAccumulator;
	float m_EnergyInterval;
	bool bIsEnergyTickActive = false;
	FString LastSavedTime; // 서버에서 저장된 마지막 시간
private:
	void IncreaseEnergy();
	void SaveEnergyStateToServer(); // 서버에 상태 저장
	
};

#define g_PSCharacterMgrValid (g_GameGlobal->IsValidManager<UPSCharacterMgr>())
#define g_PSCharacterMgr (g_GameGlobal->GetManager<UPSCharacterMgr>())