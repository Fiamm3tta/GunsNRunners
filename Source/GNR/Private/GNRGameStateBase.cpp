// Fill out your copyright notice in the Description page of Project Settings.


#include "GNRGameStateBase.h"
#include "Types/GNRStructTypes.h"

void AGNRGameStateBase::StartStage()
{
	StageStartTime = GetServerWorldTimeSeconds();
	StageEndTime = 0.f;
	TurretKillCount = 0;
	EnemyKillCount = 0;
}

void AGNRGameStateBase::FinishStage(bool bCleared)
{
	if (!bCleared)
	{
		return;
	}

	StageEndTime = GetServerWorldTimeSeconds();
}

float AGNRGameStateBase::GetElapsedTime() const
{

	if (StageStartTime <= 0.f)
	{
		return 0.f;
	}

	return GetServerWorldTimeSeconds() - StageStartTime;
}

FGNRStageRunResult AGNRGameStateBase::BuildStageRunResult()
{
	FGNRStageRunResult Result;
	Result.ClearTime = StageEndTime - StageStartTime;

	return Result;
}
