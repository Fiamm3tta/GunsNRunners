// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GNREnemyGameplayAbility.h"
#include "Characters/GNREnemyCharacter.h"

AGNREnemyCharacter* UGNREnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedGNREnemyCharacter.IsValid())
	{
		CachedGNREnemyCharacter = Cast<AGNREnemyCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedGNREnemyCharacter.IsValid() ? CachedGNREnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UGNREnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
