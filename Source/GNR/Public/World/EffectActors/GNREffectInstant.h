// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/EffectActors/GNREffectActorBase.h"
#include "GNREffectInstant.generated.h"

/**
 * 
 */
UCLASS()
class GNR_API AGNREffectInstant : public AGNREffectActorBase
{
	GENERATED_BODY()
	
protected:
	virtual void HandleEffectBeginOverlap(AActor* TargetActor);
};
