// Fill out your copyright notice in the Description page of Project Settings.


#include "PSActor_MonsterWeapon.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "ActorLibrary/Actor/Component/ActorComponent/CSStaticMeshComponent.h"
#include "ActorLibrary/Actor/Component/ActorComponent/CSBoxComponent.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "ProtoSurvivor/Character/PSCharacterBase.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"

APSActor_MonsterWeapon::APSActor_MonsterWeapon()
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
		StaticMeshComp->SetupAttachment(RootComponent);
	}
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	if (NiagaraComp)
	{
		NiagaraComp->SetupAttachment(RootComponent);
	}
}

void APSActor_MonsterWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(g_StageMgrValid)
	{
		g_StageMgr->WaveClear.AddUObject(this, &APSActor_MonsterWeapon::DestroyWeapon);
		
		FString strNiagaraName;
		if (g_StageMgr->GetStageNumber() % 5 <= 2)
		{
			strNiagaraName = g_CSDefineRecordMgr->GetValue_String("ui_monster_weapon_1");
		}
		else
		{
			strNiagaraName = g_CSDefineRecordMgr->GetValue_String("ui_monster_weapon_2");
		}
		
		if (UNiagaraSystem* pNiagara = g_FileMgr->LoadObject<UNiagaraSystem>(nResourceType::General, *strNiagaraName))
		{
			if (NiagaraComp)
			{
				NiagaraComp->SetAsset(pNiagara);
			}
		}
	}

}

void APSActor_MonsterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsActorBeingDestroyed())
	{
		FVector targetLocation = GetActorLocation() + GetActorRotation().Vector().GetSafeNormal() * GetWeaponTickSpeed();
		SetActorLocation(targetLocation);
	}
}

void APSActor_MonsterWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (APSCharacterBase* character = Cast<APSCharacterBase>(OtherActor))
	{
		character->SetCurrentHP_Damage(m_fDamage);
		DestroyWeapon();
	}
}

void APSActor_MonsterWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(g_StageMgrValid)
	{
		g_StageMgr->WaveClear.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}


void APSActor_MonsterWeapon::SetTargetLocation(FVector TargetLocation)
{
	SetActorRotation((TargetLocation - GetActorLocation()).GetSafeNormal().Rotation());
}

float APSActor_MonsterWeapon::GetWeaponTickSpeed() const
{
	if (g_PSCombatMgrValid)
	{
		return m_fSpeed * g_PSCombatMgr->GetCombatSpeed() * g_GameGlobal->GetCurWorld()->GetDeltaSeconds();
	}
	
	return 0.0f;
}

void APSActor_MonsterWeapon::DestroyWeapon()
{
	if (!IsActorBeingDestroyed())
	{
		SetActorEnableCollision(false);
		Destroy();
	}
}
