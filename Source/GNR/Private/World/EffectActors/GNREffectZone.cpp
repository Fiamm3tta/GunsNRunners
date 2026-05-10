// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EffectActors/GNREffectZone.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"

void AGNREffectZone::HandleEffectBeginOverlap(AActor* TargetActor)
{
    if (!IsValid(TargetActor))
    {
        return;
    }

    if (ActiveEffectHandles.Contains(TargetActor))
    {
        return;
    }

    const FActiveGameplayEffectHandle Handle = ApplyEffectToTarget(TargetActor);

    if (Handle.IsValid())
    {
        ActiveEffectHandles.Add(TargetActor, Handle);
        BP_OnEffectApplied(TargetActor);
    }

}

void AGNREffectZone::HandleEffectEndOverlap(AActor* TargetActor)
{
    if (!IsValid(TargetActor))
    {
        return;
    }

    FActiveGameplayEffectHandle* FoundHandle = ActiveEffectHandles.Find(TargetActor);

    if (!FoundHandle)
    {
        return;
    }

    RemoveEffectFromTarget(TargetActor, *FoundHandle);

    ActiveEffectHandles.Remove(TargetActor);

    BP_OnEffectRemoved(TargetActor);
}