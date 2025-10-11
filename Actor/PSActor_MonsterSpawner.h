// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSActor_MonsterSpawner.generated.h"

UCLASS()
class PROTOSURVIVOR_API APSActor_MonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	APSActor_MonsterSpawner();

	UPROPERTY()
	TArray<FString> m_arrMonsters;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetIsSpawn(bool _isEnable) {m_IsSpawn = _isEnable;}
	
private:
	void SpawnMonster();
	
private:
	float m_fSpawnCooldown;

	bool m_IsSpawn;
};
