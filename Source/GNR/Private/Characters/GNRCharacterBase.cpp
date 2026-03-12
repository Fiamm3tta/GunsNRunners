// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GNRCharacterBase.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"

// Sets default values
AGNRCharacterBase::AGNRCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	GNRAbilitySystemComponent = CreateDefaultSubobject<UGNRAbilitySystemComponent>(TEXT("GNRAbilitySystemComponent"));
}

UAbilitySystemComponent* AGNRCharacterBase::GetAbilitySystemComponent() const
{
	return GetGNRAbilitySystemComponent();
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
