// Fill out your copyright notice in the Description page of Project Settings.

#include "PSActor_Weapon.h"

#include "NiagaraComponent.h"
#include "ActorLibrary/Actor/Component/ActorComponent/CSStaticMeshComponent.h"
#include "ActorLibrary/Actor/Component/ActorComponent/CSBoxComponent.h"
#include "ProtoSurvivor/Character/PSMonsterBase.h"
#include "ProtoSurvivor/Define/PSCommon_Weapon.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "ProtoSurvivor/Weapon/WeaponType.h"
#include "NiagaraSystem.h"
#include "PSActor_WeaponHitSound.h"

APSActor_Weapon::APSActor_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UCSBoxComponent>(TEXT("BoxComp"));
	if (BoxComp)
	{
		SetRootComponent(BoxComp);
	}
	StaticMeshComp = CreateDefaultSubobject<UCSStaticMeshComponent>(TEXT("StaticMeshComp"));
	if (StaticMeshComp)
	{
		StaticMeshComp->SetupAttachment(GetRootComponent());
	}

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	if(NiagaraComponent)
	{
		NiagaraComponent->SetupAttachment(GetRootComponent());
	}
}

void APSActor_Weapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (APSMonsterBase* monster = Cast<APSMonsterBase>(OtherActor))
	{
		if (GetOwner() != monster)
		{
			float fCharacterStatDamage = (100+g_PSCharacterMgr->GetCharacterStat()->m_fPlusDamage)*0.01f;
			float damage = m_WeaponStat->GetTotalDamage()*fCharacterStatDamage;
			float RandomChance = FMath::FRandRange(0.0f, 100.0f);

			bool isCritical = RandomChance <= m_WeaponStat->GetTotalCriticalPercent() ? true : false;
			for (auto& weaponType : m_WeaponTypes)
			{
				weaponType->ApplyCritical(isCritical);
				weaponType->HitMonster(monster);
			}


			if (isCritical)
			{
				damage *= m_WeaponStat->GetTotalCriticalDamage();
				monster->SetCurrentHP(monster->GetCurrentHP() - damage, true);
			}
			else
			{
				monster->SetCurrentHP(monster->GetCurrentHP() - damage, false);
			}

			if (!m_WeaponStat->m_bIsThrough)
			{
				DestroyWeapon();
			}
			else
			{
				m_WeaponStat->m_fDamage = m_WeaponStat->m_fDamage*g_CSDefineRecordMgr->GetValue_Number("GV_THROUGH_DAMAGE_SUB");
			}
		}
	}
}

void APSActor_Weapon::BeginPlay()
{
	Super::BeginPlay();

	if(g_StageMgrValid)
	{
		g_StageMgr->WaveClear.AddUObject(this, &APSActor_Weapon::DestroyWeapon);
	}

	
}

void APSActor_Weapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (auto& weaponType: m_WeaponTypes)
	{
		delete[] weaponType;
	}

	if(g_StageMgrValid)
	{
		g_StageMgr->WaveClear.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

void APSActor_Weapon::DestroyWeapon()
{
	if (!IsActorBeingDestroyed())
	{
		SetActorEnableCollision(false);
		Destroy();
	}
}

void APSActor_Weapon::InitWeapon(FWeaponStat* WeaponStat)
{
	if (WeaponStat)
	{
		m_WeaponStat = WeaponStat;
		
		InitWeaponRotation();
		if (StaticMeshComp)
		{
			FString strMeshname = m_WeaponStat->m_strMeshName+ FString::FromInt(m_WeaponStat->UpgradeLevel);
			if (UStaticMesh* pMesh = g_FileMgr->LoadObject<UStaticMesh>(nResourceType::General, FName(*strMeshname)))
			{
				StaticMeshComp->SetStaticMesh(pMesh);
			}
			FString strNiagaraName = m_WeaponStat->m_strNiagaraName+ FString::FromInt(m_WeaponStat->UpgradeLevel);
			if(UNiagaraSystem* pNiagara = g_FileMgr->LoadObject<UNiagaraSystem>(nResourceType::General,*strNiagaraName))
			{
				if(NiagaraComponent)
				{
					NiagaraComponent->SetAsset(pNiagara);
				}
			}
		}
		
		for (auto& weaponType: m_WeaponStat->m_eWeaponTypes)
		{
			switch (weaponType)
			{
			case nWeaponType::Default:
				m_WeaponTypes.Emplace(new DefaultType(this));
				break;
			case nWeaponType::Split:
				m_WeaponTypes.Emplace(new SplitType(this));
				break;
			case nWeaponType::Splash:
				m_WeaponTypes.Emplace(new SplashType(this));
				break;
			case nWeaponType::Through:
				m_WeaponTypes.Emplace(new ThroughType(this));
				break;
			case nWeaponType::Continuing:
				m_WeaponTypes.Emplace(new ContinuingType(this));
				break;
			case nWeaponType::Slow:
				m_WeaponTypes.Emplace(new SlowType(this));
				break;
			case nWeaponType::Frozen:
				m_WeaponTypes.Emplace(new FrozenType(this));
				break;
			case nWeaponType::Bounce:
				m_WeaponTypes.Emplace(new BounceType(this));
				break;
			case nWeaponType::Double:
				m_WeaponTypes.Emplace(new DoubleType(this));
			default:
				break;
			}
		}
		for (auto& weaponType: m_WeaponTypes)
		{
			weaponType->SpawnWeapon();
		}
	}
}

void APSActor_Weapon::ApplyProperties()
{
	if (g_PSPropertiesMgrValid && m_WeaponStat)
	{
		m_WeaponStat->m_fDamage = (m_WeaponStat->UpgradeLevel != 1) ? m_WeaponStat->m_fDamage : m_WeaponStat->GetLevelDamage();

		m_WeaponStat->m_fDamage *= g_PSPropertiesMgr->m_fGlobalDamageBuff;

		float CriticalBoost = (g_PSPropertiesMgr->m_fGlobalCriticalBuff - 1.0f) * 100.0f;
		m_WeaponStat->m_fCriticalPercent += CriticalBoost;

		UE_LOG(LogTemp, Warning, TEXT("무기 = 데미지:%.2f*%.2f*%.2f, 쿨타임 = %.3f, 치명타확률 = %.2f%%, 글로벌 공격력 버프: %.2f"),
			   m_WeaponStat->m_fDamage,
			   (100+g_PSCharacterMgr->GetCharacterStat()->m_fPlusDamage)*0.01f,
			   m_WeaponStat->m_fDamageBuff,
			   m_WeaponStat->GetTotalCoolTime(),
			   m_WeaponStat->GetTotalCriticalPercent(),
			   g_PSPropertiesMgr->m_fGlobalDamageBuff);
		
	}
}

void APSActor_Weapon::InitWeaponRotation()
{
	TArray<FHitResult> hitResults;
	bool detectResult = g_GameGlobal->GetCurWorld()->SweepMultiByChannel(hitResults, m_StartLocation, m_StartLocation, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeBox(FVector(1500.f, m_WeaponStat->m_fRange, 1000.f)));
	if (detectResult)
	{
		if (m_WeaponStat->m_eTargetType == nTargetType::Near)
		{
			APSMonsterBase* nearestMonster = nullptr;
			float minDistance = FLT_MAX;
			for (auto& hitResult: hitResults)
			{
				if (APSMonsterBase* monster = Cast<APSMonsterBase>(hitResult.GetActor()))
				{
					float distance = UE::Geometry::Distance(m_StartLocation, monster->GetActorLocation());
					if (distance < minDistance)
					{
						minDistance = distance;
						nearestMonster = monster;
					}
				}
			}

			if (nearestMonster)
			{
				FVector targetLocation = GetActorLocation();
				targetLocation.Z = nearestMonster->GetActorLocation().Z;
				SetActorLocation(targetLocation);
				SetActorRotation((nearestMonster->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation());
			}
		}
		else if (m_WeaponStat->m_eTargetType == nTargetType::Random)
		{
			int32 randomIndex = FMath::RandRange(0, hitResults.Num() - 1);
			if (APSMonsterBase* monster = Cast<APSMonsterBase>(hitResults[randomIndex].GetActor()))
			{
				FVector targetLocation = GetActorLocation();
				targetLocation.Z = monster->GetActorLocation().Z;
				SetActorLocation(targetLocation);
				SetActorRotation((monster->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation());
			}
		}	
	}
}


float APSActor_Weapon::GetWeaponTickSpeed() const
{
	if (g_PSCombatMgrValid)
	{
		return m_WeaponStat->m_fSpeed * g_PSCombatMgr->GetCombatSpeed() * g_GameGlobal->GetCurWorld()->GetDeltaSeconds();
	}
	
	return 0.0f;
}

void APSActor_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsActorBeingDestroyed())
	{
		FVector targetLocation = GetActorLocation() + GetActorRotation().Vector().GetSafeNormal() * GetWeaponTickSpeed();
		SetActorLocation(targetLocation);
		if ((GetActorLocation().Y - m_StartLocation.Y) >= m_WeaponStat->m_fRange)
		{
			DestroyWeapon();
		}
	}
}
