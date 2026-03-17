// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GNRAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GNRFunctionLibrary.h"
#include "GNRGameplayTags.h"
#include "Interfaces/PawnCombatInterface.h"

UGNRAttributeSet::UGNRAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitBaseSpeed(1.f);
	InitCurrentSpeed(1.f);
	InitMaxSpeed(1.f);
	InitMinSpeed(1.f);
	InitAttackPower(1.f);
}

void UGNRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentSpeedAttribute())
	{
		const float NewCurrentSpeed = FMath::Clamp(GetCurrentSpeed(), GetMinSpeed(), GetMaxSpeed());

		SetCurrentSpeed(NewCurrentSpeed);
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
		/*
		UE_LOG(LogTemp, Display, 
			TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth);
		*/
	}

	if (GetCurrentHealth() == 0.f)
	{

	}
}
