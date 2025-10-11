// Fill out your copyright notice in the Description page of Project Settings.

#include "PSActor_MonsterSpawner.h"

#include "CoreLibrary/GameGlobal/CSGameGlobal.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProtoSurvivor/Character/PSBossMonster.h"
#include "ProtoSurvivor/Character/PSMonsterBase.h"
#include "ProtoSurvivor/DataTable/DataManager/PSMonsterRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSMonsterRecord.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"

APSActor_MonsterSpawner::APSActor_MonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	m_fSpawnCooldown = g_CSDefineRecordMgr->GetValue_Number("GV_SPAWN_TIMER");
	m_IsSpawn = false;
}

void APSActor_MonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(g_StageMgrValid)
	{
		g_StageMgr->m_pMonsterSpawner = this;
	}
}

void APSActor_MonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CombatDeltaTime = DeltaTime * g_PSCombatMgr->GetCombatSpeed();
	
	if(m_IsSpawn)
	{
		if(m_arrMonsters.Num() >0)
		{
			m_fSpawnCooldown += CombatDeltaTime;
			if (m_fSpawnCooldown >= g_CSDefineRecordMgr->GetValue_Number("GV_SPAWN_TIMER"))
			{
				SpawnMonster();
				m_fSpawnCooldown = 0.0f;
			}
		}
		else
		{
			m_IsSpawn = false;
			m_fSpawnCooldown = g_CSDefineRecordMgr->GetValue_Number("GV_SPAWN_TIMER");
		}
	}
}

void APSActor_MonsterSpawner::SpawnMonster()
{
	if(m_arrMonsters.IsValidIndex(0))
	{
		FString strMonster = m_arrMonsters[0];
		if(PSMonsterRecord* pRecord = g_MonsterRecordMgr->FindRecord(strMonster))
		{
			FVector spawnLocation = GetActorLocation();
			FRotator spawnRotation = GetActorRotation();

			float RandomX = UKismetMathLibrary::RandomFloatInRange(-g_StageMgr->GetXLocation(),g_StageMgr->GetXLocation()); 
			spawnLocation.X += RandomX;
			
			if (m_arrMonsters.Num() == 1 &&
				(g_StageMgr->GetWaveNumber() + 1 == g_StageMgr->m_pSpawnRecord->m_WaveNumber ||
				  (g_StageMgr->GetWaveNumber()+1 >= 10&&(g_StageMgr->GetWaveNumber() + 1) % 5 == 0)))
			{
				if (UClass* monsterClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "PSBossMonster_BP"))
				{
					if (APSBossMonster* bossMonster = g_GameGlobal->GetCurWorld()->SpawnActor<APSBossMonster>(monsterClass, spawnLocation, spawnRotation))
					{
						bossMonster->SetMonsterProperty(pRecord, m_arrMonsters.Num());
						m_arrMonsters.RemoveAt(0);
					}
				}
			}
			else
			{
				if (UClass* monsterClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, *pRecord->m_strBPName))
				{
					if (APSMonsterBase* monster =
						g_GameGlobal->GetCurWorld()->SpawnActor<APSMonsterBase>(monsterClass, spawnLocation, spawnRotation))
					{
						monster->SetMonsterProperty(pRecord,m_arrMonsters.Num());
						m_arrMonsters.RemoveAt(0);
					}
				}
			}
		}
	}
}