// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponPickUpComponent.h"
#include "Characters/GNRPlayerCharacter.h"

UWeaponPickUpComponent::UWeaponPickUpComponent()
{
    InitSphereRadius(32.f);
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SetGenerateOverlapEvents(true);
}

void UWeaponPickUpComponent::BeginPlay()
{
    Super::BeginPlay();

    OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
}

void UWeaponPickUpComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // UE_LOG(LogTemp, Warning, TEXT("Pickup Overlap OtherActor: %s"), *GetNameSafe(OtherActor));
    AGNRPlayerCharacter* PlayerCharacter = Cast<AGNRPlayerCharacter>(OtherActor);
    if (!PlayerCharacter)
    {
        return;
    }

    OnWeaponPickedUp.Broadcast(PlayerCharacter);

    SetCollisionEnabled(ECollisionEnabled::NoCollision);
    OnComponentBeginOverlap.RemoveAll(this);
}
