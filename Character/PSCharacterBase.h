
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PSCharacterBase.generated.h"

class UCapsuleComponent;
class UAnimMontage;
class UMaterialInstanceDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCurrentHPChanged, float, NewCurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMaxHPChanged, float, NewMaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCurrentArmorChanged, float, NewCurrentArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMaxArmorChanged, float, NewMaxArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCurrentEXPChanged, float, NewCurrentEXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMaxEXPChanged, float, NewMaxEXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSilverChanged, int32, NewSilver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSelectPropertiesed);

UCLASS()
class PROTOSURVIVOR_API APSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* AttackAnimMontage;
	
	UPROPERTY()
	UMaterialInstanceDynamic* OverlayMaterial;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	APSCharacterBase();

	void SetCurrentHP_Damage(float Damage);
	void SetCurrentHP_Heal(float Heal);
	float GetCurrentHP() const {return m_fCurrentHP;}

	void SetMaxHP();
	float GetMaxHP() const {return m_fMaxHP;}

	void SetArmorHP(float NewArmorHP);
	float GetArmorHP() const {return m_fArmorHP;}

	void SetCurrentArmor(float NewCurrentArmor);
	float GetCurrentArmor() const {return m_fCurrentArmor;}

	void SetMaxArmor(float NewMaxArmor);
	float GetMaxArmor() const {return m_fMaxArmor;}

	void SetCurrentEXP(float NewCurrentEXP);
	float GetCurrentEXP() const {return m_fCurrentEXP;}

	void SetMaxEXP(float NewMaxEXP);
	float GetMaxEXP() const {return m_fMaxEXP;}

	void AddSilver(int32 NewSilver);
	int32 GetSilver() const {return m_iSilver;}

	void SetRespawn(bool Respawn) {m_bUsedRespawn = Respawn;}
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SetCharacterPlay();
	void WaveClear();

public:
	FOnPlayerCurrentHPChanged OnPlayerCurrentHPChanged;
	FOnPlayerMaxHPChanged OnPlayerMaxHPChanged;
	FOnPlayerCurrentArmorChanged OnPlayerCurrentArmorChanged;
	FOnPlayerMaxArmorChanged OnPlayerMaxArmorChanged;
	FOnPlayerCurrentEXPChanged OnPlayerCurrentEXPChanged;
	FOnPlayerMaxEXPChanged OnPlayerMaxEXPChanged;
	FOnPlayerSelectPropertiesed OnPlayerSelectPropertiesed;
	FOnPlayerSilverChanged OnPlayerSilverChanged;
private:
	// m_fCharacterHP + m_fArmorHP
	float m_fCurrentHP = 0.f;
	float m_fMaxHP = 0.f;

	// 기존 캐릭터 HP
	float m_fCharacterHP = 0.f;
	// 방어구 아이템 HP
	float m_fArmorHP = 0.f;
	
	float m_fCurrentArmor = 0.f;
	float m_fMaxArmor = 0.f;

	float m_fCurrentEXP = 0.f;
	float m_fMaxEXP = 0.f;

	float m_iSilver;

	bool m_bUsedRespawn;

	bool m_bIsDamaged = false;
	float m_fDamageEffectCooldown = 0.3f;
	float m_fDamageEffectSecond = 0.f;
};