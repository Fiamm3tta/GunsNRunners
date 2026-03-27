// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GNRProjectileWeapon.h"
#include "Projectile/GNRProjectile.h"
#include "Characters/GNRPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void AGNRProjectileWeapon::FireInternal()
{
	SpawnProjectile();
}

void AGNRProjectileWeapon::SkillInternal()
{

}

void AGNRProjectileWeapon::SpawnProjectile()
{
    if (!ProjectileClass) return;
    if (!OwningPlayerCharacter) return;

    const FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
    const FVector ShootDirection = GetDirectionToAimPoint();
    const FRotator SpawnRotation = ShootDirection.Rotation();
    const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

    AGNRProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGNRProjectile>(
        ProjectileClass,
        SpawnTransform,
        OwningPlayerCharacter,
        OwningPlayerCharacter
    );

    if (!Projectile) return;

    Projectile->InitProjectileData(DamageEffectClass);

    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

}
