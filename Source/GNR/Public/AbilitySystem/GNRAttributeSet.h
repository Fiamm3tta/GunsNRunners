// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "GNRAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GNR_API UGNRAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UGNRAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Speed")
	FGameplayAttributeData BaseSpeed;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, BaseSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Speed")
	FGameplayAttributeData CurrentSpeed;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, CurrentSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Speed")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, MaxSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UGNRAttributeSet, DamageTaken)
};
