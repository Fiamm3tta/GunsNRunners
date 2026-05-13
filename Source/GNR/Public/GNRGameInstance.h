// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GNRGameInstance.generated.h"

struct FGNRStageRunResult;
struct FGNRStageClearRecord;
class UGNRSaveGame;
class UDataAsset_MapData;

USTRUCT(BlueprintType)
struct FGNRGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FName StageId;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return StageId.IsValid() && !Level.IsNull();
	}
};

/**
 * 
 */
UCLASS()
class GNR_API UGNRGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGNRGameLevelSet> GameLevelSets;

public:
	UFUNCTION(BlueprintPure, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelById(FName InStageId) const;

	UPROPERTY(BlueprintReadWrite)
	FName CurrentStageId;

	UPROPERTY(BlueprintReadWrite)
	bool CurrentbHard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataAsset_MapData> MapDataAsset;

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadOrCreateSaveGame();

	UFUNCTION(BlueprintCallable)
	bool SaveToDisk();

	UFUNCTION(BlueprintCallable)
	void SaveStageResult(const FGNRStageRunResult& Result);

	UFUNCTION(BlueprintCallable)
	int32 GetStarCount(FName StageId) const;

	UFUNCTION(BlueprintCallable)
	bool GetHardStar(FName StageId) const;

private:
	UPROPERTY()
	TObjectPtr<UGNRSaveGame> CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	FString SaveSlotName = TEXT("GunsNRunners_Save_0");

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	int32 UserIndex = 0;

};
