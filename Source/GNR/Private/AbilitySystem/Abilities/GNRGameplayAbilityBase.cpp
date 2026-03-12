// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GNRGameplayAbilityBase.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGNRGameplayAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EGNRAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UGNRGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EGNRAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UGNRAbilitySystemComponent* UGNRGameplayAbilityBase::GetGNRAbilitySystemComponentFromActorInfo() const
{
	return Cast<UGNRAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}