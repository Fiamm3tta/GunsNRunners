// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GNRStructTypes.generated.h"

class UGNRGameplayAbilityBase;

USTRUCT(BlueprintType)
struct FGNRPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGNRGameplayAbilityBase> AbilityToGrant;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FGNRStageRunResult
{
	GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName StageId;

    UPROPERTY(BlueprintReadWrite)
    bool bCleared = false;

    UPROPERTY(BlueprintReadWrite)
    bool bHardMode = false;

    UPROPERTY(BlueprintReadWrite)
    float ClearTime = 0.f;

    UPROPERTY(BlueprintReadWrite)
    int32 StarCount = 0;
};