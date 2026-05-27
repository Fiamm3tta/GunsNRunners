// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GNRCharacterBase.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "AbilitySystem/GNRAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
AGNRCharacterBase::AGNRCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	GNRAbilitySystemComponent = CreateDefaultSubobject<UGNRAbilitySystemComponent>(TEXT("GNRAbilitySystemComponent"));
	GNRAttributeSet = CreateDefaultSubobject<UGNRAttributeSet>(TEXT("GNRAttributeSet"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AGNRCharacterBase::GetAbilitySystemComponent() const
{
	return GetGNRAbilitySystemComponent();
}

UPawnCombatComponent* AGNRCharacterBase::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AGNRCharacterBase::GetPawnUIComponent() const
{
	return nullptr;
}

void AGNRCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (GNRAbilitySystemComponent)
	{
		GNRAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
	}
}
