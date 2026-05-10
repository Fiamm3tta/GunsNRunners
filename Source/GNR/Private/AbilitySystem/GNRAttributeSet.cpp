// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GNRAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GNRFunctionLibrary.h"
#include "GNRGameplayTags.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"

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
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}

	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("Couldn't extract a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentSpeedAttribute())
	{
		const float NewCurrentSpeed = FMath::Clamp(GetCurrentSpeed(), GetMinSpeed(), GetMaxSpeed());

		SetCurrentSpeed(NewCurrentSpeed);

		PawnUIComponent->OnCurrentSpeedChanged.Broadcast(GetCurrentSpeed());
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
		
		UE_LOG(LogTemp, Display, 
			TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth);
		
	}

	PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth());

	if (GetCurrentHealth() == 0.f)
	{
		UGNRFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), GNRGameplayTags::Shared_Status_Death);
	}
}