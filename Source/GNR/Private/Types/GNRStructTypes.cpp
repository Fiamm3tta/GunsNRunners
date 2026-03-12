// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/GNRStructTypes.h"
#include "AbilitySystem/Abilities/GNRPlayerGameplayAbility.h"

bool FGNRPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
