// Fill out your copyright notice in the Description page of Project Settings.

#include "PSCharacterMgr.h"

#include "PSBackPackMgr.h"
#include "PSDataBaseMgr.h"
#include "Database/DatabaseReference.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/DataTable/DataManager/PSCharacterStatDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataManager/PSWeaponDetailRecordMgr.h"
#include "ProtoSurvivor/DataTable/DataRecord/PSCharacterStatDetailRecord.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
const int32 GOLD_MAX = 1000;

void UPSCharacterMgr::SetMaxHP(float NewMaxHP)
{
	if(m_CharacterStat)
	{
		m_CharacterStat->m_fMaxHP = NewMaxHP;
	}
}

float UPSCharacterMgr::GetMaxHP() const
{

	float fHP = g_CSDefineRecordMgr->GetValue_Number("GV_CHARACTERHP_DEFAULT") + g_PSCharacterMgr->GetCharacterStat()->m_fMaxHP;
	// 실험용

	return fHP;
}

int32 UPSCharacterMgr::GetLevel(const FString& _name)
{
	int32 Level = 0;
	if(m_MapCharacterLevel.Contains(_name))
	{
		Level = m_MapCharacterLevel[_name];
	}

	return Level;
}

FString UPSCharacterMgr::GetstrDia()
{
	FString strDia;
	TArray<FString> arrDiaType = {"K","M","B"};
	int32 dia = m_Dia;
	float fDia = 0.f;
	int32 Count  = 0;
	while(dia / GOLD_MAX >= 1)
	{
		fDia = ((float)dia)/1000.f;
		dia = dia/1000;
		Count++;
	}

	if(Count == 0)
	{
		strDia = FString::FromInt(m_Dia);
	}
	else
	{
		strDia = FString::SanitizeFloat(fDia,2);
		FString strLeft;
		FString strRight;
		strDia.Split(TEXT("."),&strLeft,&strRight);
		strRight =strRight.Left(2);
		strDia = strLeft + TEXT(".") + strRight;
	
		if(arrDiaType.IsValidIndex(Count-1))
		{
			strDia += arrDiaType[Count-1];
		}
	}
	
	return strDia;
}

void UPSCharacterMgr::SetDia(int32 _dia)
{
	m_Dia = _dia;

	if(ChangedGold.IsBound())
	{
		ChangedGold.Broadcast(m_Dia,nGoodsType::Dia);
	}

	g_DataBaseMgr->SetCharacterGoods(TEXT("Dia"),m_Dia);
}

FString UPSCharacterMgr::GetstrGold()
{
	FString strGold;
	TArray<FString> arrGoldType = {"K","M","B"};
	int32 gold = m_Gold;
	float fGold = 0.f;
	int32 Count  = 0;
	while(gold / GOLD_MAX >= 1)
	{
		fGold = ((float)gold)/1000.f;
		gold = gold/1000;
		Count++;
	}

	if(Count == 0)
	{
		strGold = FString::FromInt(m_Gold);
	}
	else
	{
		strGold = FString::SanitizeFloat(fGold,2);
		FString strLeft;
		FString strRight;
		strGold.Split(TEXT("."),&strLeft,&strRight);
		strRight =strRight.Left(2);
		strGold = strLeft + TEXT(".") + strRight;
	
		if(arrGoldType.IsValidIndex(Count-1))
		{
			strGold += arrGoldType[Count-1];
		}
	}
	
	return strGold;
}

void UPSCharacterMgr::SetGold(int32 _gold)
{
	m_Gold = _gold;

	if(ChangedGold.IsBound())
	{
		ChangedGold.Broadcast(m_Gold,nGoodsType::Gold);
	}

	g_DataBaseMgr->SetCharacterGoods(TEXT("Gold"),m_Gold);
}

FString UPSCharacterMgr::GetstrEnergy()
{
	FString strEnergy;
	TArray<FString> arrEnergyType = {"K", "M", "B"};
	int32 energy = m_Energy;
	float fEnergy = 0.f;
	int32 Count = 0;
	while (energy / GOLD_MAX >= 1)
	{
		fEnergy = ((float)energy) / 1000.f;
		energy = energy / 1000;
		Count++;
	}

	if (Count == 0)
	{
		strEnergy = FString::FromInt(m_Energy);
	}
	else
	{
		strEnergy = FString::SanitizeFloat(fEnergy, 2);
		FString strLeft;
		FString strRight;
		strEnergy.Split(TEXT("."), &strLeft, &strRight);
		strRight = strRight.Left(2);
		strEnergy = strLeft + TEXT(".") + strRight;

		if (arrEnergyType.IsValidIndex(Count - 1))
		{
			strEnergy += arrEnergyType[Count - 1];
		}
	}

	if(m_Energy < m_MaxEnergy)
	{
		StartEnergyTick(m_EnergyInterval);
	}
	
	FString strMaxEnergy = FString::FromInt(m_MaxEnergy);
	
	return strEnergy + TEXT("/") + strMaxEnergy;
}

void UPSCharacterMgr::SetEnergy(int32 _energy)
{
	m_Energy = _energy;

	if(ChangedGold.IsBound())
	{
		ChangedGold.Broadcast(m_Energy,nGoodsType::Energy);
	}

	SaveEnergyStateToServer();
	g_DataBaseMgr->SetCharacterGoods(TEXT("Energy"),m_Energy);
}

void UPSCharacterMgr::StartEnergyTick(float EnergyInterval)
{
	m_EnergyInterval = EnergyInterval;
	bIsEnergyTickActive = true;
}

void UPSCharacterMgr::StopEnergyTick()
{
	bIsEnergyTickActive = false;
	m_EnergyAccumulator = m_EnergyInterval;
}

void UPSCharacterMgr::IncreaseEnergy()
{
	if (m_Energy < m_MaxEnergy)
	{
		m_Energy++;
		
		if(m_Energy >= m_MaxEnergy)
		{
			StopEnergyTick();
		}
		
		SetEnergy(m_Energy);
	}
}

void UPSCharacterMgr::SaveEnergyStateToServer()
{
	if (!g_DataBaseMgrValid)
		return;

	FDateTime CurrentUtcTime = FDateTime::UtcNow();
	FDateTime CurrentKstTime = CurrentUtcTime + FTimespan(0, 9, 0, 0);
	FString LastSavedTimeKST = CurrentKstTime.ToString();

	int32 AccumulatorToSave = static_cast<int32>(m_EnergyAccumulator > 0 ? m_EnergyAccumulator : m_EnergyInterval);

	g_DataBaseMgr->SetCharacterGoodsString(TEXT("LastSavedTime"), LastSavedTimeKST);
	g_DataBaseMgr->SetCharacterGoods(TEXT("LastSavedSeconds"), AccumulatorToSave);

	UE_LOG(LogTemp, Warning, TEXT("999_Saving LastSavedTime: %s"), *LastSavedTimeKST);
	UE_LOG(LogTemp, Warning, TEXT("999_Saving Accumulator: %d"), AccumulatorToSave);
}



void UPSCharacterMgr::SetPurchasedAdRemove(bool bPurchasedAdRemove)
{
	m_bPurchasedAdRemove = bPurchasedAdRemove;

	g_DataBaseMgr->SetPurchaseInfo(TEXT("AdRemove"), m_bPurchasedAdRemove);
}

void UPSCharacterMgr::UnlockWeapons(const FString& _strTid)
{
	if(m_MapWeapon.Contains(_strTid))
	{
		m_MapWeapon[_strTid]->m_IsLock = true;
		m_MapWeapon[_strTid]->Level = 1;
		g_DataBaseMgr->SetWeaponUnLock(_strTid);
	}
}

void UPSCharacterMgr::ChangeEquipWeapon(int32 number, FWeaponStat* _weaponstat,bool _isEquip)
{
	if(_weaponstat)
	{
		if(!_isEquip)
		{
			m_mapEquipWeapon.Remove(number);
		}
		else
		{
			m_mapEquipWeapon.Emplace(number,_weaponstat);
		}

		if(m_MapWeapon.Contains(_weaponstat->m_strTid))
		{
			m_MapWeapon[_weaponstat->m_strTid]->m_IsEquip= _isEquip;

			if(g_DataBaseMgrValid)
			{
				g_DataBaseMgr->SetWeaponEquip(_weaponstat->m_strTid,_isEquip);
			}
		}
	}
}

void UPSCharacterMgr::TestPointer()
{
	m_MapWeapon["Test_weapon1"]->Level++;
}


void UPSCharacterMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	m_CharacterStat  =new FCharacterStat();
}

void UPSCharacterMgr::Deinitialize()
{
	if(g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
	}
	
	SaveEnergyStateToServer();
	
	Super::Deinitialize();
}

void UPSCharacterMgr::Load()
{
	Super::Load();
	///// Test /////////////
	///
	//m_pWeaponStat = g_WeaponDetailRecordMgr->FindRecord("Sword")->m_WeaponStat;
	///////////////////////
	InitCharacterStat();
	InitWeapon();
	m_Gold = g_CSDefineRecordMgr->GetValue_Number("0");
	m_MaxEnergy = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_ENERGY_END");
	m_EnergyInterval = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_ENERGY_TIME");
	m_Dia = g_CSDefineRecordMgr->GetValue_Number("0");

	if (g_DataBaseMgr->GetIsLoad())
	{
		ProcessLoadedData();
	}
	else
	{
		g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this, &UPSCharacterMgr::ProcessLoadedData);
	}
}

void UPSCharacterMgr::ProcessLoadedData()
{
    if (!LastSavedTime.IsEmpty())
    {
        FString CorrectedLastSavedTime = LastSavedTime.Replace(TEXT("."), TEXT("-"));
        CorrectedLastSavedTime = CorrectedLastSavedTime.Left(10) + TEXT("T") + CorrectedLastSavedTime.Mid(11).Replace(TEXT("."), TEXT(":"));
        CorrectedLastSavedTime += TEXT("Z");

        FDateTime LastSavedDateTime;
        if (FDateTime::ParseIso8601(*CorrectedLastSavedTime, LastSavedDateTime))
        {
            FDateTime CurrentUtcTime = FDateTime::UtcNow();
            FDateTime CurrentKstTime = CurrentUtcTime + FTimespan(0, 9, 0, 0);

            FTimespan TimeDifference = CurrentKstTime - LastSavedDateTime;

            float SecondsElapsed = static_cast<float>(TimeDifference.GetTotalSeconds());

        	float addEnergy = SecondsElapsed / m_EnergyInterval;
        	float namageEnergy = FMath::Fmod(SecondsElapsed, m_EnergyInterval);

            float SavedAccumulator = (m_EnergyAccumulator > 0 ? m_EnergyAccumulator : m_EnergyInterval);

        	if(SavedAccumulator - namageEnergy < 0.0f)
        	{
        		SavedAccumulator += m_EnergyInterval;
        		m_EnergyAccumulator = SavedAccumulator - namageEnergy;
        		addEnergy++;
        	}
            else
            {
            	m_EnergyAccumulator = SavedAccumulator - namageEnergy;
            }

            int32 EnergyToAdd = addEnergy;

            SetEnergy(FMath::Min(m_Energy + EnergyToAdd, m_MaxEnergy));

            if (m_Energy >= m_MaxEnergy)
            {
                m_EnergyAccumulator = m_EnergyInterval;
            }
        }
    }
}

void UPSCharacterMgr::SetLastSavedTime(const FString& SavedTime)
{
	LastSavedTime = SavedTime;
}

void UPSCharacterMgr::OnMoveGoodsStore(int32 menuNumber)
{
	if(MoveGoodsStore.IsBound())
	{
		MoveGoodsStore.Execute(menuNumber);
	}
}

void UPSCharacterMgr::Release()
{
	Super::Release();
}

void UPSCharacterMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsEnergyTickActive)
	{
		m_EnergyAccumulator -= DeltaTime;

		if (m_EnergyAccumulator <= 0.0f && m_Energy < m_MaxEnergy)
		{
			m_EnergyAccumulator += m_EnergyInterval;
			IncreaseEnergy();
		}
	}
}

void UPSCharacterMgr::InitCharacterStat()
{
	if(m_CharacterStat)
	{
		m_CharacterStat->m_fMaxHP = GetStatValue("Health");
		m_CharacterStat->m_fPlusDamage = GetStatValue("Damage");
		m_CharacterStat->m_fArmor = GetStatValue("Armor");
		m_CharacterStat->m_fPlusCoolTime = GetStatValue("CoolDown");
		m_CharacterStat->m_fSubMonsterMoveTime = GetStatValue("MonsterMove");
		m_CharacterStat->m_fSubMonsterAttackTime = GetStatValue("MonsterAttack");

		UE_LOG(LogTemp,Warning,TEXT("%f  %f  %f  %f  %f  %f"),m_CharacterStat->m_fMaxHP,m_CharacterStat->m_fPlusDamage
			,m_CharacterStat->m_fArmor,m_CharacterStat->m_fPlusCoolTime,m_CharacterStat->m_fSubMonsterMoveTime,m_CharacterStat->m_fSubMonsterAttackTime)

		if(RefreshStat.IsBound())
		{
			RefreshStat.Broadcast();
		}
	}


}

void UPSCharacterMgr::InitCharacterGoods(UDataSnapshot* _dataSnapShot)
{
	if(_dataSnapShot)
	{
		m_Gold = _dataSnapShot->GetChild("Gold")->GetValue().AsInt32();
		m_Dia = _dataSnapShot->GetChild("Dia")->GetValue().AsInt32();
		m_Energy = _dataSnapShot->GetChild("Energy")->GetValue().AsInt32();
		m_EnergyAccumulator = _dataSnapShot->GetChild("LastSavedSeconds")->GetValue().AsInt32();

		if(RefreshGoods.IsBound())
		{
			RefreshGoods.Broadcast();
		}
	}
}

void UPSCharacterMgr::InitWeapon()
{
	TArray<PSWeaponDetailRecord*> arrWeapons;

	g_WeaponDetailRecordMgr->GetAllRecord(arrWeapons);
	
	int32 Number = 0;
	for(PSWeaponDetailRecord* pRecord : arrWeapons)
	{
		
		if(pRecord->m_WeaponStat->m_IsLock)
		{
			pRecord->m_WeaponStat->m_IsEquip = true;
			pRecord->m_WeaponStat->Level = 1;
			m_mapEquipWeapon.Emplace(Number,pRecord->m_WeaponStat);
			Number++;
		}
		m_MapWeapon.Emplace(pRecord->m_strTid.ToString(),pRecord->m_WeaponStat);
	}
}

void UPSCharacterMgr::InitPurchaseInfo(UDataSnapshot* _dataSnapShot)
{
	if (_dataSnapShot)
	{
		m_bPurchasedAdRemove = _dataSnapShot->GetChild("AdRemove")->GetValue().AsBool();
	}
}


void UPSCharacterMgr::OnRefreshWeapon()
{
	if(RefreshWeapon.IsBound())
	{
		RefreshWeapon.Broadcast();
	}
}



float UPSCharacterMgr::GetStatValue(const FString& _name)
{
	float fValue = 0.f;
	int32 level = 0;
	FString strTid;
	nCharacterStat::en eCharacterStat = nCharacterStat::GetEnum(_name,false);

	if(!m_MapCharacterLevel.Contains(_name))
	{
		m_MapCharacterLevel.Emplace(_name,1);
	}

	level = m_MapCharacterLevel[_name];

	strTid = _name + TEXT("_") + FString::FromInt(level);
	if(PSCharacterStatDetailRecord* pRecord = g_CharacterStatDetailRecordMgr->FindRecord(strTid))
	{
		fValue = pRecord->m_fValue;
	}
	
	return fValue;
}

void UPSCharacterMgr::SetStatValue(const FString& _name, int32 _level)
{
	if(m_MapCharacterLevel.Contains(_name))
	{
		m_MapCharacterLevel.Emplace(_name,_level);
	}
}

void UPSCharacterMgr::SetWeaponPiece(const FString& tid, int32 piece)
{
	if(m_MapWeapon.Contains(tid))
	{
		m_MapWeapon[tid]->Piece += piece;

		g_DataBaseMgr->SetWeaponPiece(tid,m_MapWeapon[tid]->Piece);
	}
}

float UPSCharacterMgr::GetCharacterStatCoolTime()
{
	float fCooltime = 1.f;
	if(m_CharacterStat)
	{
		fCooltime = (100-m_CharacterStat->m_fPlusCoolTime)*0.01f;
	}

	return fCooltime;
}

float UPSCharacterMgr::GetCharacterStatArmor()
{
	float fArmor = 0.f;
	if(m_CharacterStat)
	{
		fArmor = (100-m_CharacterStat->m_fArmor)*0.01f;
	}

	return fArmor;
}

float UPSCharacterMgr::GetCharacterStatMonsterMove()
{
	float fMonsterMove = 1.f;
	if(m_CharacterStat)
	{
		fMonsterMove = (100-m_CharacterStat->m_fSubMonsterMoveTime)*0.01f;
	}

	return fMonsterMove;
}

float UPSCharacterMgr::GetCharacterStatMonsterAttack()
{
	float fMonsterAttack = 1.f;
	if(m_CharacterStat)
	{
		fMonsterAttack = (100+m_CharacterStat->m_fSubMonsterAttackTime)*0.01f;
	}

	return fMonsterAttack;
}
