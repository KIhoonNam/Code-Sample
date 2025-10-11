// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProtoSurvivor/Manager/PSPropertiesMgr.h"
#include "PSActor_MonsterWeapon.generated.h"

class UCSStaticMeshComponent;
class UCSBoxComponent;
class UNiagaraComponent;

UCLASS()
class PROTOSURVIVOR_API APSActor_MonsterWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UCSStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
	UCSBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComp;
	
public:	
	APSActor_MonsterWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetTargetLocation(FVector TargetLocation);
	void SetDamage(float Damage) { m_fDamage = Damage; }
	void SetSpeed(float Speed) { m_fSpeed = Speed * g_PSPropertiesMgr->m_fGlobalMAttackSpeedDeBuff;}

private:
	float GetWeaponTickSpeed() const;

	void DestroyWeapon();
	
private:
	float m_fDamage;
	float m_fSpeed;
};
