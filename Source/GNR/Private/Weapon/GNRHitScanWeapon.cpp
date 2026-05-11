// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GNRHitScanWeapon.h"
#include "Characters/GNRPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void AGNRHitScanWeapon::FireInternal()
{
	PerformHitscan();
}

void AGNRHitScanWeapon::SkillStartInternal()
{
    Super::SkillStartInternal();

    if (OwningPlayerCharacter)
    {
        OwningPlayerCharacter->StartZoom();
    }
}

void AGNRHitScanWeapon::SkillEndInternal()
{
    Super::SkillEndInternal();

    if (OwningPlayerCharacter)
    {
        OwningPlayerCharacter->StopZoom();
    }
}


void AGNRHitScanWeapon::PerformHitscan()
{
    if (!OwningPlayerCharacter)
    {
        return;
    }

    const FVector Start = MuzzlePoint->GetComponentLocation();
    const FVector AimPoint = GetAimPoint(HitScanRange);
    const FVector Direction = (AimPoint - Start).GetSafeNormal();
    const FVector End = Start + Direction * HitScanRange;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwningPlayerCharacter);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPlayerCharacter);
            UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);

            if (SourceASC && TargetASC && DamageEffectClass)
            {
                FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
                EffectContext.AddSourceObject(this);
                EffectContext.AddHitResult(Hit);

                FGameplayEffectSpecHandle SpecHandle =
                    SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);

                if (SpecHandle.IsValid())
                {
                    SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
                }
            }
        }

        DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.5f);
        DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 8.0f, FColor::Yellow, false, 1.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.5f);
    }
}
