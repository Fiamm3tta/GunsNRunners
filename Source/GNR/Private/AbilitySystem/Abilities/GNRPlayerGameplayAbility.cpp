// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GNRPlayerGameplayAbility.h"
#include "Characters/GNRPlayerCharacter.h"
#include "Controllers/GNRMainPlayerController.h"

AGNRPlayerCharacter* UGNRPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!CachedGNRPlayerCharacter.IsValid())
	{
		CachedGNRPlayerCharacter = Cast<AGNRPlayerCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedGNRPlayerCharacter.IsValid() ? CachedGNRPlayerCharacter.Get() : nullptr;
}

AGNRMainPlayerController* UGNRPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedGNRPlayerController.IsValid())
	{
		CachedGNRPlayerController = Cast<AGNRMainPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedGNRPlayerController.IsValid() ? CachedGNRPlayerController.Get() : nullptr;
}
