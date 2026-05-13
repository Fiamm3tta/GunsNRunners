// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GNRGameStateBase.generated.h"

struct FGNRStageRunResult;
/**
 * 
 */
UCLASS()
class GNR_API AGNRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
    void StartStage();
    void FinishStage(bool bCleared);
    void AddTurretKillCount() { TurretKillCount++; }

    UFUNCTION(BlueprintCallable)
    void AddEnemyKillCount() { EnemyKillCount++; }

    int32 GetTurretKillCount() { return TurretKillCount; }
    int32 GetEnemyKillCount() { return EnemyKillCount; }

    UFUNCTION(BlueprintCallable)
    float GetElapsedTime() const;
    
    FGNRStageRunResult BuildStageRunResult();

private:
    float StageStartTime = 0.f;
    float StageEndTime = 0.f;
    int32 TurretKillCount = 0;
    int32 EnemyKillCount = 0;
};
