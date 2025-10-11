// Fill out your copyright notice in the Description page of Project Settings.

#include "PSCharacterBase.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreLibrary/Manager/CSFileManager.h"
#include "CoreLibrary/Manager/CSSoundMgr.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProtoSurvivor/Actor/PSActor_WeaponHitSound.h"
#include "ProtoSurvivor/Animation/PSAnimInstance.h"
#include "ProtoSurvivor/Controller/PSPlayerController.h"
#include "ProtoSurvivor/Manager/PSBackPackMgr.h"
#include "ProtoSurvivor/Manager/PSCharacterMgr.h"
#include "ProtoSurvivor/Manager/PSCombatMgr.h"
#include "ProtoSurvivor/Manager/PSDataBaseMgr.h"
#include "ProtoSurvivor/Manager/PSPauseMgr.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "ProtoSurvivor/Manager/PSStageMgr.h"
#include "TableLibrary/DataTable/DataManager/CSDefineRecordMgr.h"
#include "UILibrary/Define/CSDefine_UICommon.h"
#include "UILibrary/Manager/CSUIMgr.h"
#include "ProtoSurvivor/UI/Monster/PSWidget_Damage.h"
#include "ProtoSurvivor/Weapon/ArmorProperty.h"
#include "UILibrary/Widget/BaseWidget/CSTextBlock.h"

APSCharacterBase::APSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
}

void APSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (g_DataBaseMgrValid)
	{
		if (g_DataBaseMgr->GetIsLoad())
		{
			SetCharacterPlay();
		}
		else
		{
			g_DataBaseMgr->LoadCompleteDataBase.AddUObject(this, &APSCharacterBase::SetCharacterPlay);
		}
	}
	
	if(g_StageMgrValid)
	{
		g_StageMgr->WaveClear.AddUObject(this, &APSCharacterBase::WaveClear);
	}

	if(g_PSPropertiesMgrValid)
	{
		g_PSPropertiesMgr->ResetGlobalBuff();
		g_PSPropertiesMgr->ResetWeaponBuff();
	}

	//임시
	if(g_PSBackPackMgrValid)
	{
		g_PSBackPackMgr->ActiveCoolDownSlots.Empty();
	}

	if (UMaterialInstance* pMaterialInstance = g_FileMgr->LoadObject<UMaterialInstance>(nResourceType::General, "MI_MonsterEffect"))
	{
		OverlayMaterial = UMaterialInstanceDynamic::Create(pMaterialInstance, this);
		if (OverlayMaterial)
		{
			GetMesh()->SetOverlayMaterial(OverlayMaterial);
		}
	}
}

void APSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CombatDeltaTime = DeltaTime * g_PSCombatMgr->GetCombatSpeed();
	
	if (m_bIsDamaged)
	{
		m_fDamageEffectSecond += CombatDeltaTime;
		if (m_fDamageEffectSecond >= m_fDamageEffectCooldown)
		{
			m_bIsDamaged = false;
			m_fDamageEffectSecond = 0.f;
			if (OverlayMaterial)
			{
				OverlayMaterial->SetScalarParameterValue(FName(TEXT("Opacity")),0.f);
			}
		}
	}
}

void APSCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (g_DataBaseMgrValid)
	{
		g_DataBaseMgr->LoadCompleteDataBase.RemoveAll(this);
	}
	
	if(g_StageMgrValid)
	{
		g_StageMgr->WaveClear.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}


void APSCharacterBase::SetCharacterPlay()
{
	if (g_StageMgrValid)
	{
		if (g_PSCharacterMgrValid)
		{
			m_fCharacterHP = g_PSCharacterMgr->GetMaxHP();
			SetArmorHP(0.f);
			SetCurrentHP_Heal(m_fMaxHP);
			SetCurrentArmor(0.f);

			float expAmount = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_EXP_AMOUNT");
			if (g_StageMgr->GetStageNumber() > 0)
			{
				float expUpgradeValue = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_EXP_UPGRADE_VALUE");
				expAmount += expAmount * g_StageMgr->GetStageNumber() * expUpgradeValue;
			}
			
			SetMaxEXP(expAmount);
			SetCurrentEXP(0.f);
		}
	}
}

void APSCharacterBase::WaveClear()
{
	if (UPSAnimInstance* AnimInstance = Cast<UPSAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		if (AnimInstance->IsAnyMontagePlaying())
		{
			AnimInstance->StopAllMontages(0.1f);
		}
	}

	float HPPercent = m_fCurrentHP / m_fMaxHP;
	SetArmorHP(0.f);
	SetCurrentArmor(0.f);
	m_fCurrentHP = m_fMaxHP * HPPercent;
}

void APSCharacterBase::SetCurrentHP_Damage(float Damage)
{
	if (g_PSPropertiesMgrValid)
	{
		TArray<ArmorProperty*> armorProperties = g_PSPropertiesMgr->m_arrArmorProperties;
		if (armorProperties.IsValidIndex(0))
		{
			for (auto& armorProperty : armorProperties)
			{
				Damage = armorProperty->DecreaseDamage(Damage, m_fCurrentHP / m_fMaxHP);
			}
		}
	}

	Damage = UKismetMathLibrary::Clamp(Damage*g_PSCharacterMgr->GetCharacterStatArmor(),0,Damage*g_PSCharacterMgr->GetCharacterStatArmor()+1);
	if (m_fCurrentArmor >= Damage)
	{
		SetCurrentArmor(m_fCurrentArmor - Damage);
	}
	else
	{
		m_fCurrentHP -= Damage - m_fCurrentArmor;
		SetCurrentArmor(0.f);
	}

	if(APSActor_WeaponHitSound* pSound = GetWorld()->SpawnActor<APSActor_WeaponHitSound>(APSActor_WeaponHitSound::StaticClass()))
	{
		pSound->PlaySound("Sound_MonsterAttack_Cue");
	}
	
	if (UClass* pWidgetClass = g_FileMgr->LoadObject<UClass>(nResourceType::UClass, "W_Damage_BP"))
	{
		if (APSPlayerController* pPlayerController = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			if (UPSWidget_Damage* pDamageWidget = CreateWidget<UPSWidget_Damage>(GetWorld(), pWidgetClass))
			{
				FVector2D screenPosition;
				if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(pPlayerController, GetActorLocation(), screenPosition, false))
				{
					pDamageWidget->DamageText->SetText(FText::FromString(FString::Printf(TEXT("-%0.f"), Damage)));

					screenPosition += FVector2D(0.f, -50.f);
					pDamageWidget->SetPositionInViewport(screenPosition, false);
					pDamageWidget->AddToViewport(-1);
					if (g_PSCombatMgrValid)
					{
						pDamageWidget->PlayAnimation(pDamageWidget->PopupAnim, 0.f, 1, EUMGSequencePlayMode::Forward, g_PSCombatMgr->GetCombatSpeed());
					}
				}
			}
		}
	}
	
	if (m_fCurrentHP <= 0.f)
	{
		m_fCurrentHP = 0.f;
		if (g_UIMgrValid)
		{
			if (!m_bUsedRespawn)
			{
				g_UIMgr->ShowUIPanel(nUIPanelType::Respawn);
			}
			else
			{
				g_UIMgr->ShowUIPanel(nUIPanelType::StageClear);
			}
		}
	}

	if (OverlayMaterial)
	{
		m_bIsDamaged = true;
		OverlayMaterial->SetScalarParameterValue(FName(TEXT("Opacity")),1.f);
	}
	
	if (OnPlayerCurrentHPChanged.IsBound())
	{
		OnPlayerCurrentHPChanged.Broadcast(m_fCurrentHP);
	}
}

void APSCharacterBase::SetCurrentHP_Heal(float Heal)
{
	m_fCurrentHP += Heal;

	if (m_fCurrentHP >= m_fMaxHP)
	{
		m_fCurrentHP = m_fMaxHP;
	}
	
	if (OnPlayerCurrentHPChanged.IsBound())
	{
		OnPlayerCurrentHPChanged.Broadcast(m_fCurrentHP);
	}
}

void APSCharacterBase::SetMaxHP()
{
	m_fMaxHP = m_fCharacterHP + m_fArmorHP;
	SetMaxArmor(m_fMaxHP);
	
	if (OnPlayerMaxHPChanged.IsBound())
	{
		OnPlayerMaxHPChanged.Broadcast(m_fMaxHP);
	}
}

void APSCharacterBase::SetArmorHP(float NewArmorHP)
{
	m_fArmorHP = NewArmorHP;
	
	SetMaxHP();
}

void APSCharacterBase::SetCurrentArmor(float NewCurrentArmor)
{
	if (NewCurrentArmor >= m_fMaxArmor)
	{
		m_fCurrentArmor = m_fMaxArmor;
	}
	else
	{
		m_fCurrentArmor = NewCurrentArmor;
	}

	if (m_fCurrentArmor <= 0.f)
	{
		m_fCurrentArmor = 0.f;
	}
	
	if (OnPlayerCurrentArmorChanged.IsBound())
	{
		OnPlayerCurrentArmorChanged.Broadcast(m_fCurrentArmor);
	}
}

void APSCharacterBase::SetMaxArmor(float NewMaxArmor)
{
	m_fMaxArmor = m_fMaxHP;

	if (OnPlayerMaxArmorChanged.IsBound())
	{
		OnPlayerMaxArmorChanged.Broadcast(m_fMaxArmor);
	}
}

void APSCharacterBase::SetCurrentEXP(float NewCurrentEXP)
{
	m_fCurrentEXP = NewCurrentEXP;
	if (m_fCurrentEXP >= m_fMaxEXP)
	{
		if (OnPlayerSelectPropertiesed.IsBound())
		{
			OnPlayerSelectPropertiesed.Broadcast();
		}
		
		m_fCurrentEXP -= m_fMaxEXP;
		float levelUpEXP = g_CSDefineRecordMgr->GetValue_Number("GV_STAGE_EXP_LEVEL_UP");
		SetMaxEXP(m_fMaxEXP * levelUpEXP);
	}

	if (OnPlayerCurrentEXPChanged.IsBound())
	{
		OnPlayerCurrentEXPChanged.Broadcast(m_fCurrentEXP);
	}
}

void APSCharacterBase::SetMaxEXP(float NewMaxEXP)
{
	m_fMaxEXP = NewMaxEXP;

	if (OnPlayerMaxEXPChanged.IsBound())
	{
		OnPlayerMaxEXPChanged.Broadcast(m_fMaxEXP);
	}
}

void APSCharacterBase::AddSilver(int32 NewSilver)
{
	m_iSilver = GetSilver() + NewSilver;

	if (OnPlayerSilverChanged.IsBound())
	{
		OnPlayerSilverChanged.Broadcast(m_iSilver);
	}
}


