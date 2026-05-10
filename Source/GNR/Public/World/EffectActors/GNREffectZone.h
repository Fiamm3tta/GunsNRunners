// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/EffectActors/GNREffectActorBase.h"
#include "GameplayEffectTypes.h"
#include "GNREffectZone.generated.h"

/**
 * 
 */
UCLASS()
class GNR_API AGNREffectZone : public AGNREffectActorBase
{
	GENERATED_BODY()
	
protected:
	TMap<TWeakObjectPtr<AActor>, FActiveGameplayEffectHandle> ActiveEffectHandles;

	virtual void HandleEffectBeginOverlap(AActor* TargetActor);
	virtual void HandleEffectEndOverlap(AActor* TargetActor);
};
