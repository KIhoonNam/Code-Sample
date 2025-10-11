// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSActor_Weapon.generated.h"

class APSMonsterBase;
struct FWeaponStat;
class UCSStaticMeshComponent;
class UNiagaraComponent;
class UCSBoxComponent;
class WeaponType;

UCLASS()
class PROTOSURVIVOR_API APSActor_Weapon : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	FString m_strHitSound;
	
	UPROPERTY(EditAnywhere)
	UCSStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
	UCSBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComponent;
public:	
	APSActor_Weapon();
	
	void InitWeapon(FWeaponStat* WeaponStat);
	void ApplyProperties();
	void InitWeaponRotation();

	FWeaponStat* GetWeaponStat() const  {return m_WeaponStat;}

	FVector GetStartLocation() const {return m_StartLocation;}
	void SetStartLocation(FVector Location) {m_StartLocation = Location;}
	
	float GetWeaponTickSpeed() const;

	TSet<WeaponType*> GetWeaponTypes(){return m_WeaponTypes;}
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void DestroyWeapon();
	
private:
	FWeaponStat* m_WeaponStat;
	TSet<WeaponType*> m_WeaponTypes;

	FVector m_StartLocation;
};
