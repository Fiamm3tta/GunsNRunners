// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_MapData.generated.h"

USTRUCT(BlueprintType)
struct FMapData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName StageId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SubTurretKillCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SubEnemyKillCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SubTimeLimit = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> PreviewImage;
};

/**
 * 
 */
UCLASS()
class GNR_API UDataAsset_MapData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMapData> MapDatas;

	UFUNCTION(BlueprintPure)
	FMapData GetMapDataByStageId(FName InStageId) const;

};
