// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/GNRSpecialProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"

void AGNRSpecialProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode(Hit);
}

void AGNRSpecialProjectile::Explode(const FHitResult& Hit)
{
	const FVector ExplosionOrigin = GetActorLocation();

	DamageEnemiesInRadius(ExplosionOrigin);
	DestroyProjectilesInRadius(ExplosionOrigin);

	Destroy();
}

void AGNRSpecialProjectile::DamageEnemiesInRadius(const FVector& Origin)
{
	UWorld* World = GetWorld();
	if (!World || !ExplosionDamageEffectClass)
	{
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(
		World,
		Origin,
		ExplosionRadius,
		ObjectTypes,
		EnemyClassFilter,
		ActorsToIgnore,
		OverlappedActors
	);

	UAbilitySystemComponent* SourceASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());

	if (!SourceASC)
	{
		SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	}

	for (AActor* TargetActor : OverlappedActors)
	{
		if (!IsValid(TargetActor))
		{
			continue;
		}

		if (TargetActor == GetOwner() || TargetActor == GetInstigator())
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!TargetASC)
		{
			continue;
		}

		if (SourceASC)
		{
			FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			EffectContext.AddHitResult(FHitResult());

			FGameplayEffectSpecHandle SpecHandle =
				SourceASC->MakeOutgoingSpec(ExplosionDamageEffectClass, 1.0f, EffectContext);

			if (SpecHandle.IsValid())
			{
				SourceASC->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(),
					TargetASC
				);
			}
		}
	}
}

void AGNRSpecialProjectile::DestroyProjectilesInRadius(const FVector& Origin)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// 현재 투사체 Collision Object Type이 WorldDynamic이면 이걸 사용
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(
		World,
		Origin,
		ExplosionRadius,
		ObjectTypes,
		ProjectileClassFilter,
		ActorsToIgnore,
		OverlappedActors
	);

	for (AActor* Actor : OverlappedActors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		if (Actor == this)
		{
			continue;
		}

		Actor->Destroy();
	}
}