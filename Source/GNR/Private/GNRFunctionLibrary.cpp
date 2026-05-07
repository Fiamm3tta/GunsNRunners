// Fill out your copyright notice in the Description page of Project Settings.


#include "GNRFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

UGNRAbilitySystemComponent* UGNRFunctionLibrary::NativeGetGNRASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UGNRAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UGNRFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UGNRAbilitySystemComponent* ASC = NativeGetGNRASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UGNRFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UGNRAbilitySystemComponent* ASC = NativeGetGNRASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UGNRFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UGNRAbilitySystemComponent* ASC = NativeGetGNRASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UGNRFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EGNRConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EGNRConfirmType::Yes : EGNRConfirmType::No;;
}

UPawnCombatComponent* UGNRFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	
	return nullptr;
}

UPawnCombatComponent* UGNRFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EGNRValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EGNRValidType::Valid : EGNRValidType::Invalid;

	return CombatComponent;
}