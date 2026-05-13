// Fill out your copyright notice in the Description page of Project Settings.


#include "GNRGameInstance.h"
#include "Types/GNRStructTypes.h"
#include "SaveGame/GNRSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/DataAsset_MapData.h"

TSoftObjectPtr<UWorld> UGNRGameInstance::GetGameLevelById(FName InStageId) const
{
	for (const FGNRGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid()) continue;

		if (GameLevelSet.StageId == InStageId)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}

void UGNRGameInstance::Init()
{
	Super::Init();

	LoadOrCreateSaveGame();
}

void UGNRGameInstance::LoadOrCreateSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex);
		CurrentSaveGame = Cast<UGNRSaveGame>(LoadedSave);
	}

	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UGNRSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UGNRSaveGame::StaticClass())
		);

		SaveToDisk();
	}
}

bool UGNRGameInstance::SaveToDisk()
{
	if (!CurrentSaveGame)
	{
		return false;
	}

	return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, UserIndex);
}

void UGNRGameInstance::SaveStageResult(const FGNRStageRunResult& Result)
{	
	if (!CurrentSaveGame)
	{
		LoadOrCreateSaveGame();
	}

	if (!CurrentSaveGame || Result.StageId.IsNone())
	{
		UE_LOG(LogTemp, Display, TEXT("No Save Game"));
		return;
	}

	FGNRStageClearRecord& Record = CurrentSaveGame->StageRecords.FindOrAdd(Result.StageId);

	if (Result.bCleared)
	{
		if (Result.bHardMode)
		{
			Record.bHardCleard = true;
		}
		else
		{
			Record.bNormalCleared = true;
		}

		if (Record.BestClearTime <= 0.f || Result.ClearTime < Record.BestClearTime)
		{
			Record.BestClearTime = Result.ClearTime;
		}

		Record.BestStarCount = FMath::Max(Record.BestStarCount, Result.StarCount);
	}

	UE_LOG(LogTemp, Display, TEXT("ClearTime: %f, StarCount: %d"), Result.ClearTime, Result.StarCount);

	SaveToDisk();
}

int32 UGNRGameInstance::GetStarCount(FName StageId) const
{
	if (!CurrentSaveGame || StageId.IsNone())
	{
		return 0;
	}

	const FGNRStageClearRecord* StageData = CurrentSaveGame->StageRecords.Find(StageId);

	if (!StageData)
	{
		return 0;
	}

	return StageData->BestStarCount;
}

bool UGNRGameInstance::GetHardStar(FName StageId) const
{
	if (!CurrentSaveGame || StageId.IsNone())
	{
		return 0;
	}

	const FGNRStageClearRecord* StageData = CurrentSaveGame->StageRecords.Find(StageId);

	if (!StageData)
	{
		return 0;
	}

	return StageData->bHardCleard;
}
