// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GNRWeaponBase.h"
#include "GNRHitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GNR_API AGNRHitScanWeapon : public AGNRWeaponBase
{
	GENERATED_BODY()
	
protected:
	void FireInternal() override;
	void SkillInternal() override;

	void PerformHitscan();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Hitscan")
	float HitScanRange = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Hitscan")
	float Damage = 1.0f;
};
