// Fill out your copyright notice in the Description page of Project Settings.


#include "PSStageMgr.h"

#include "NiagaraActor.h"
#include "PSBackPackMgr.h"
#include "PSCharacterMgr.h"
#include "PSCombatMgr.h"
#include "PSDataBaseMgr.h"
#include "PSPropertiesMgr.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/DataTable/DataManager/PSStageRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataManager/PSStageSpawnRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSStageSpawnRecord.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Manager/CSGameMsgMgr.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "UILibrary/Widget/CSUIUtils.h"

void UPSStageMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPSStageMgr::Deinitialize()
{
	Super::Deinitialize();
}

void UPSStageMgr::Load()
{
	Super::Load();

	if (g_StageRecordMgr)
	{
		g_StageRecordMgr->GetAllRecord(m_arrStageRecord);
	}
}

void UPSStageMgr::Release()
{
	Super::Release();
}

void UPSStageMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UPSStageMgr::StageSetting()
{
	m_IsClear = false;
	m_bIsGameEnded = false;
	m_fBossKillHP = 1.f;
	m_fBossKillDamage = 1.f;
	FString strStageNumber = FString::FromInt(m_StageNumber);
	if(PSStageSpawnRecord* pRecord = g_StageSpawnRecordMgr->FindRecord(strStageNumber))
	{
		m_pSpawnRecord = pRecord;
		m_WaveNumber = 0;
		
		if (OnMaxWaveNumberChanged.IsBound())
		{
			OnMaxWaveNumberChanged.Broadcast(m_pSpawnRecord->m_WaveNumber);
		}
		if (OnWaveNumberChanged.IsBound())
		{
			OnWaveNumberChanged.Broadcast(m_WaveNumber);
		}
		if(WaveClear.IsBound())
		{
			WaveClear.Broadcast();
		}

		if (g_PSCombatMgrValid)
		{
			g_PSCombatMgr->SetCombatSpeed(1.0f);
		}
		if(g_PSBackPackMgrValid)
		{
			g_PSBackPackMgr->SetNearWeaponAdded(false);
		}
	}
}

void UPSStageMgr::CreateSpawnXLocation()
{
	FVector position;
	FVector Direction;
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetWorld(),0),FVector2D(0,0),position,Direction);

	if(ACharacter* pCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		if(UStaticMeshComponent* pCamera = Cast<UStaticMeshComponent>(pCharacter->GetComponentByClass(UStaticMeshComponent::StaticClass())))
		{
			position.Z -= 1.0f;
			FRotator FindLookAt = UKismetMathLibrary::FindLookAtRotation(pCamera->GetComponentLocation(),position);

			pCamera->SetWorldRotation(FindLookAt);

			FHitResult Result;
			TArray<AActor*> ignoreActor;
			FVector EndLocation =(position - pCamera->GetComponentLocation())  * 100000.f;
			UKismetSystemLibrary::LineTraceSingle(GetWorld(),pCamera->GetComponentLocation(),EndLocation,TraceTypeQuery1,false,ignoreActor,EDrawDebugTrace::None,Result,true);

			if(Result.bBlockingHit)
			{
				SpawnXLocation = Result.Location.X;
			}
		}
	}
}

void UPSStageMgr::PlusMonster()
{
	m_CurrentMonster++;

	if(m_CurrentMonster >= m_MaxMonster)
	{
		DeleteNiagara();
		NextWave();
	}
}

void UPSStageMgr::NextWave()
{
	if (m_pSpawnRecord)
	{
		m_WaveNumber++;
		FString stageKey = "Stage_" + FString::FromInt(m_StageNumber);
		if (m_arrStageHighWave.Find(stageKey) == nullptr)
		{
			m_arrStageHighWave.Emplace(stageKey, 1);
		}
		if (m_arrStageHighWave[stageKey] < m_WaveNumber)
		{
			m_arrStageHighWave[stageKey] = m_WaveNumber;
			if (g_DataBaseMgrValid)
			{
				g_DataBaseMgr->SetStageHighWave(m_StageNumber, m_WaveNumber);
			}			
		}
		
		if (m_WaveNumber < m_pSpawnRecord->m_WaveNumber)
		{
			if(WaveClear.IsBound())
			{
				WaveClear.Broadcast();
			}
		}
		else
		{
			m_IsClear = true;
			if(g_UIMgrValid)
			{
				g_UIMgr->ShowUIPanel(nUIPanelType::StageClear);
			}
			if (m_StageNumber == m_MaxStageNumber)
			{
				m_StageNumber++;
				m_MaxStageNumber++;
				if (g_DataBaseMgrValid)
				{
					g_DataBaseMgr->SetStageInfo(m_MaxStageNumber);
				}
			}
		}
	}
}

void UPSStageMgr::DeleteNiagara()
{
	TArray<AActor*> arrActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ANiagaraActor::StaticClass(),arrActors);

	for(AActor* pActor : arrActors)
	{
		pActor->Destroy();
	}
}

void UPSStageMgr::UpgradeBossValue()
{
	m_fBossKillDamage += g_CSDefineRecordMgr->GetValue_Number("GV_MONSTER_DAMAGE_UPGRADE_VALUE_BOSSKILL");
	m_fBossKillHP += g_CSDefineRecordMgr->GetValue_Number("GV_MONSTER_HP_UPGRADE_VALUE_BOSSKILL");
}

float UPSStageMgr::GetMaxWaveNumber()
{
	float maxWaveNumber = 0;

	if(m_pSpawnRecord)
	{
		maxWaveNumber = m_pSpawnRecord->m_WaveNumber;
	}

	return maxWaveNumber;
}

void UPSStageMgr::WaveSetting()
{
	if(m_pSpawnRecord)
	{
		if(m_pSpawnRecord->arrWaves.IsValidIndex(m_WaveNumber))
		{
			FString strWaveSpawn = m_pSpawnRecord->arrWaves[m_WaveNumber];
			TArray<FString> arrMonsterTypes;
			strWaveSpawn.ParseIntoArray(arrMonsterTypes,TEXT(","));
			TArray<FString> arrWaveMonster;
			for(const FString& MonsterType : arrMonsterTypes)
			{
				FString strMonsterTid;
				FString strMonsterNumber;

				MonsterType.Split(TEXT(":"),&strMonsterTid,&strMonsterNumber);

				int32 MonsterNumber = FCString::Atoi(*strMonsterNumber);

				for(int Number=  0; Number<MonsterNumber;Number++)
				{
					arrWaveMonster.Emplace(strMonsterTid);
				}
			}
			m_MaxMonster = arrWaveMonster.Num();
			m_CurrentMonster = 0;
			if(m_pMonsterSpawner.IsValid())
			{
				m_pMonsterSpawner->m_arrMonsters = arrWaveMonster;
			}

			if (OnWaveNumberChanged.IsBound())
			{
				OnWaveNumberChanged.Broadcast(m_WaveNumber);
			}
		}
	}
}

void UPSStageMgr::WaveStart()
{
	if (m_WaveNumber + 1 == m_pSpawnRecord->m_WaveNumber ||
	(m_WaveNumber + 1 >= 10 && (m_WaveNumber + 1) % 5 == 0))
	{
		if(g_GameMsgMgrValid)
		{
			g_GameMsgMgr->ShowMessageBox("Popup_Boss_Wave");
		}
	}
	
	if(m_pMonsterSpawner.IsValid())
	{
		CreateSpawnXLocation();
		m_pMonsterSpawner->SetIsSpawn(true);
	}
}
