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