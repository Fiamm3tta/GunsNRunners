// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GNRStageGameMode.h"
#include "GNRGameInstance.h"
#include "GNRGameStateBase.h"
#include "Types/GNRStructTypes.h"
#include "DataAssets/DataAsset_MapData.h"

void AGNRStageGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	CurrentGameInstance = GetGameInstance<UGNRGameInstance>();

	if (!CurrentGameInstance)
	{
		ErrorMessage = TEXT("Invalid GNRGameInstance");
		return;
	}

	// CurrentStageId = CurrentGameInstance->CurrentStageId;
}

void AGNRStageGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameState = GetGameState<AGNRGameStateBase>();

	if (!CurrentGameState)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid GNRGameState"));
		return;
	}

	CurrentGameState->StartStage();
}

void AGNRStageGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGNRStageGameMode::HandleStageClear()
{
	if (!CurrentGameState || !CurrentGameInstance)
	{
		return;
	}
	
	CurrentGameState->FinishStage(true);
	
	FGNRStageRunResult Result = CurrentGameState->BuildStageRunResult();

	Result.bCleared = true;
	Result.bHardMode = bHard;
	Result.StageId = CurrentStageId;
	
	// 서브 미션 클리어 여부 계산 후 Result에 반영
	if (!bHard)
	{
		FMapData MapData = CurrentGameInstance->MapDataAsset->GetMapDataByStageId(CurrentStageId);
		int32 StarCount = 1;
		if (MapData.SubEnemyKillCount > 0 && MapData.SubEnemyKillCount <= CurrentGameState->GetEnemyKillCount())
		{
			StarCount++;
		}

		if (MapData.SubTurretKillCount > 0 && MapData.SubTurretKillCount <= CurrentGameState->GetTurretKillCount())
		{
			StarCount++;
		}

		if (MapData.SubTimeLimit > 0.f && MapData.SubTimeLimit >= Result.ClearTime)
		{
			StarCount++;
		}

		if (StarCount > 3)
		{
			StarCount = 3;
		}

		Result.StarCount = StarCount;
	}

	CurrentGameInstance->SaveStageResult(Result);
}
