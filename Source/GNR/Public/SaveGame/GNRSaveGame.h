// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GNRSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FGNRStageClearRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bNormalCleared = false;

	UPROPERTY(BlueprintReadWrite)
	bool bHardCleard = false;

	UPROPERTY(BlueprintReadWrite)
	float BestClearTime = 0.f;

	UPROPERTY(BlueprintReadWrite)
	int32 BestStarCount = 0;
};

/**
 * 
 */
UCLASS()
class GNR_API UGNRSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TMap<FName, FGNRStageClearRecord> StageRecords;
};
