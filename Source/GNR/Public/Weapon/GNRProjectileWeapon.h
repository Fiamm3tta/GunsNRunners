// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GNRWeaponBase.h"
#include "GNRProjectileWeapon.generated.h"

class AGNRProjectile;

/**
 * 
 */
UCLASS()
class GNR_API AGNRProjectileWeapon : public AGNRWeaponBase
{
	GENERATED_BODY()

protected:
	void FireInternal() override;
	void SkillInternal() override;

	void SpawnProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<AGNRProjectile> ProjectileClass;
	
};