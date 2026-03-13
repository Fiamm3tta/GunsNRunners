// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GNRProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/GNRCharacterBase.h"
#include "Weapon/GNRWeaponBase.h"

AGNRProjectileBase::AGNRProjectileBase()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGNRProjectileBase::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AGNRProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	// 플레이어, 적, 다른 투사체면 파괴
	if (ShouldDestroyOnHit(OtherActor, OtherComp))
	{
		Destroy();
		return;
	}

	// 물리 오브젝트면 impulse 주고 계속 튕기거나 파괴 정책 선택
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
}

bool AGNRProjectileBase::ShouldDestroyOnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp) const
{
	if (OtherActor->IsA(AGNRCharacterBase::StaticClass()))
	{
		return true;
	}

	if (OtherActor->IsA(AGNRWeaponBase::StaticClass()))
	{
		return true;
	}

	if (OtherActor->IsA(AGNRProjectileBase::StaticClass()))
	{
		return true;
	}

	// 이후 추가될 터렛형 적도 여기서 확인

	return false;
}
