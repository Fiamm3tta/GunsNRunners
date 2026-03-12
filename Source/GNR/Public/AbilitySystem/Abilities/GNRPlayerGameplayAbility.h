// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GNRGameplayAbilityBase.h"
#include "GNRPlayerGameplayAbility.generated.h"

class AGNRPlayerCharacter;
class AGNRMainPlayerController;

/**
 * 
 */
UCLASS()
class GNR_API UGNRPlayerGameplayAbility : public UGNRGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "GNR|Asbility")
	AGNRPlayerCharacter* GetPlayerCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "GNR|Ability")
	AGNRMainPlayerController* GetPlayerControllerFromActorInfo();
	
private:
	TWeakObjectPtr<AGNRPlayerCharacter> CachedGNRPlayerCharacter;
	TWeakObjectPtr<AGNRMainPlayerController> CachedGNRPlayerController;
	
};
