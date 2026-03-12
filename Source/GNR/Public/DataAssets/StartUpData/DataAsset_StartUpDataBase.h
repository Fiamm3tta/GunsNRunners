// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGNRGameplayAbilityBase;
class UGNRAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GNR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UGNRAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UGNRGameplayAbilityBase > > ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UGNRGameplayAbilityBase > > ReactiveAbilities;

	void GrantAbilities(const TArray< TSubclassOf < UGNRGameplayAbilityBase > >& InAbilitiesToGive, UGNRAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
