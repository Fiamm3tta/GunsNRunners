// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/GNRProjectile.h"
#include "GNRSpecialProjectile.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class GNR_API AGNRSpecialProjectile : public AGNRProjectile
{
	GENERATED_BODY()

public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void Explode(const FHitResult& Hit);
	void DamageEnemiesInRadius(const FVector& Origin);
	void DestroyProjectilesInRadius(const FVector& Origin);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	float ExplosionRadius = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	TSubclassOf<UGameplayEffect> ExplosionDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	TSubclassOf<AActor> EnemyClassFilter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	TSubclassOf<AActor> ProjectileClassFilter;
};
