// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GNRAbilitySystemComponent.h"

void UGNRAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}
void UGNRAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{

}