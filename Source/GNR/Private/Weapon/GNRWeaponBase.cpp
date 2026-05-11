// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GNRWeaponBase.h"
#include "Characters/GNRPlayerCharacter.h"
#include "Weapon/WeaponPickUpComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AGNRWeaponBase::AGNRWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(Root);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);

    MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
    MuzzlePoint->SetupAttachment(WeaponMesh);

    PickupComponent = CreateDefaultSubobject<UWeaponPickUpComponent>(TEXT("PickupComponent"));
    PickupComponent->SetupAttachment(Root);

}

void AGNRWeaponBase::OnEquipped(AGNRPlayerCharacter* NewOwnerCharacter)
{
    if (!NewOwnerCharacter)
    {
        return;
    }

    OwningPlayerCharacter = NewOwnerCharacter;
    bEquipped = true;

    SetPickupEnabled(false);
    SetWeaponCollisionEnabled(false);
    AttachToCharacterMesh(NewOwnerCharacter);

    SetActorEnableCollision(false);
}

void AGNRWeaponBase::OnUnequipped()
{
    bEquipped = false;
    OwningPlayerCharacter = nullptr;

    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AGNRWeaponBase::Fire()
{
    if (!OwningPlayerCharacter || !bEquipped)
    {
        return;
    }

    FireInternal();
}

void AGNRWeaponBase::SkillStart()
{
    if (!OwningPlayerCharacter || !bEquipped)
    {
        return;
    }

    SkillStartInternal();
}

void AGNRWeaponBase::SkillEnd()
{
    if (!OwningPlayerCharacter || !bEquipped)
    {
        return;
    }

    SkillEndInternal();
}

// Called when the game starts or when spawned
void AGNRWeaponBase::BeginPlay()
{
    Super::BeginPlay();

    if (PickupComponent)
    {
        PickupComponent->OnWeaponPickedUp.AddDynamic(this, &ThisClass::HandlePickedUp);
    }
}

void AGNRWeaponBase::HandlePickedUp(AGNRPlayerCharacter* PlayerCharacter)
{
    // UE_LOG(LogTemp, Warning, TEXT("Weapon HandlePickedUp called"));
    if (!PlayerCharacter || bEquipped)
    {
        return;
    }

    PlayerCharacter->EquipWeapon(this);
}

void AGNRWeaponBase::AttachToCharacterMesh(AGNRPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        return;
    }

    USkeletalMeshComponent* CharacterMesh = PlayerCharacter->GetMesh1P();
    if (!CharacterMesh)
    {
        return;
    }

    AttachToComponent(
        CharacterMesh,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocketName);
}

void AGNRWeaponBase::SetPickupEnabled(bool bEnabled)
{
    if (!PickupComponent)
    {
        return;
    }

    PickupComponent->SetGenerateOverlapEvents(bEnabled);
    PickupComponent->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void AGNRWeaponBase::SetWeaponCollisionEnabled(bool bEnabled)
{
    if (!WeaponMesh)
    {
        return;
    }

    WeaponMesh->SetSimulatePhysics(false);
    WeaponMesh->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void AGNRWeaponBase::FireInternal()
{
}

void AGNRWeaponBase::SkillStartInternal()
{
}

void AGNRWeaponBase::SkillEndInternal()
{
}

FVector AGNRWeaponBase::GetAimPoint(float TraceDistance) const
{
    if (!OwningPlayerCharacter) return FVector::ZeroVector;

    FVector CameraLocation;
    FRotator CameraRotation;

    OwningPlayerCharacter->GetActorEyesViewPoint(CameraLocation, CameraRotation);

    const FVector TraceEnd = CameraLocation + CameraRotation.Vector() * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwningPlayerCharacter);

    if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility, Params))
    {
        return Hit.ImpactPoint;
    }

    return TraceEnd;
}

FVector AGNRWeaponBase::GetDirectionToAimPoint() const
{
    const FVector MuzzleLocation = MuzzlePoint->GetComponentLocation();
    const FVector AimPoint = GetAimPoint(AimTraceDistance);

    return (AimPoint - MuzzleLocation).GetSafeNormal();
}
