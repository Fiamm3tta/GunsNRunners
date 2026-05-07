// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GNREnemyGameplayAbility.h"
#include "Characters/GNREnemyCharacter.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "GNRGameplayTags.h"

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

FGameplayEffectSpecHandle UGNREnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetGNRAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetGNRAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		GNRGameplayTags::Shared_SetByCaller_AttackPower,
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);

	return EffectSpecHandle;
}