// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreLibrary/Manager/CSBaseManager.h"
#include "ProtoSurvivor/Actor/PSActor_MonsterSpawner.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSStageSpawnRecord.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSStageRecord.h"
#include "PSStageMgr.generated.h"

DECLARE_MULTICAST_DELEGATE(FWaveClear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveNumberChanged, int32, NewWaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxWaveNumberChanged, int32, NewMaxWaveNumber);
/**
 * 
 */
UCLASS()
class PROTOSURVIVOR_API UPSStageMgr : public UCSBaseManager
{
	GENERATED_BODY()

public:
	PSStageSpawnRecord* m_pSpawnRecord;

	TWeakObjectPtr<APSActor_MonsterSpawner> m_pMonsterSpawner;

	FWaveClear WaveClear;

	FOnWaveNumberChanged OnWaveNumberChanged;
	FOnMaxWaveNumberChanged OnMaxWaveNumberChanged;

	TArray<PSStageRecord*> m_arrStageRecord;

	TMap<FString, int32> m_arrStageHighWave;

	float m_fBossKillHP = 1.f;
	float m_fBossKillDamage = 1.f;
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Load() override;
	virtual void Release() override;
	virtual void Tick(float DeltaTime) override;


public:
	void StageSetting();

	void WaveSetting();

	void WaveStart();

	void CreateSpawnXLocation();

	void PlusMonster();

	void NextWave();

	void DeleteNiagara();

	void UpgradeBossValue();
public:
	bool GetIsClear(){return m_IsClear;}
	
	float GetMaxWaveNumber();
	
	int32 GetStageNumber(){return m_StageNumber;}
	void SetStageNumber(int32 _stageNumber){m_StageNumber=_stageNumber;}

	int32 GetWaveNumber(){return m_WaveNumber;}
	void SetWaveNumber(int32 _waveNumber){m_WaveNumber=_waveNumber;}

	int32 GetWaveValueFive(){return m_WaveNumber/5;}
	
	float GetXLocation(){return SpawnXLocation;}

	int32 GetCurrentMonster(){return m_CurrentMonster;}

	int32 GetMaxStageNumber() const {return m_MaxStageNumber;}
	void SetMaxStageNumber(int32 _maxStageNumber){m_MaxStageNumber=_maxStageNumber;}

	bool GetIsGameEnded() const {return m_bIsGameEnded;}
	void SetIsGameEnded(bool GameEnded) {m_bIsGameEnded = GameEnded;}
private:
	int32 m_StageNumber = 1;
	int32 m_WaveNumber;

	int32 m_MaxStageNumber = 1;

	float SpawnXLocation = 0.f;

	int32 m_MaxMonster;
	int32 m_CurrentMonster;

	bool m_IsClear = false;
	bool m_bIsGameEnded = false;
};



#define g_StageMgrValid (g_GameGlobal->IsValidManager<UPSStageMgr>())
#define g_StageMgr (g_GameGlobal->GetManager<UPSStageMgr>())