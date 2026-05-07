// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GNRGameplayAbilityBase.h"
#include "GNREnemyGameplayAbility.generated.h"

class AGNREnemyCharacter;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class GNR_API UGNREnemyGameplayAbility : public UGNRGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GNR|Ability")
	AGNREnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "GNR|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();


	UFUNCTION(BlueprintPure, Category = "GNR|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<AGNREnemyCharacter> CachedGNREnemyCharacter;
};
