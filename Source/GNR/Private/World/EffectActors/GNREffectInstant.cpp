// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EffectActors/GNREffectInstant.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"


void AGNREffectInstant::HandleEffectBeginOverlap(AActor* TargetActor)
{
    const FActiveGameplayEffectHandle Handle = ApplyEffectToTarget(TargetActor);

    if (Handle.IsValid())
    {
        BP_OnEffectApplied(TargetActor);
    }
}