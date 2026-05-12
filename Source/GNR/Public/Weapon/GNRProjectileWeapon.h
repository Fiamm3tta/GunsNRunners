// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GNRWeaponBase.h"
#include "GNRProjectileWeapon.generated.h"

class AGNRProjectile;
class AGNRSpecialProjectile;

/**
 * 
 */
UCLASS()
class GNR_API AGNRProjectileWeapon : public AGNRWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void FireInternal() override;
	virtual void SkillStartInternal() override;
	virtual void SkillEndInternal() override;

	void SpawnProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<AGNRProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<AGNRSpecialProjectile> SpecialProjectileClass;
	
};