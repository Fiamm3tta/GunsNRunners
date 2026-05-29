// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EffectActors/GNREffectActorBase.h"
#include "Components/BoxComponent.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameModes/GNRStageGameMode.h"

// Sets default values
AGNREffectActorBase::AGNREffectActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EffectCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EffectCollisionBox"));
	SetRootComponent(EffectCollisionBox);
	EffectCollisionBox->InitBoxExtent(FVector(10.f, 10.f, 10.f));

    EffectCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    EffectCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    EffectCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    EffectCollisionBox->SetGenerateOverlapEvents(true);

	EffectCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnEffectBeginOverlap);
	EffectCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnEffectEndOverlap);
}

void AGNREffectActorBase::BeginPlay()
{
    Super::BeginPlay();

    if (AGNRStageGameMode* StageGameMode = GetWorld()->GetAuthGameMode<AGNRStageGameMode>())
    {
        if (StageGameMode->GetbHard())
        {
            EffectLevel = 2.f;
        }
    }
}

void AGNREffectActorBase::OnEffectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // UE_LOG(LogTemp, Display, TEXT("OverlapBegin"));
    
    HandleEffectBeginOverlap(OtherActor);
}

void AGNREffectActorBase::OnEffectEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    HandleEffectEndOverlap(OtherActor);
}

void AGNREffectActorBase::HandleEffectBeginOverlap(AActor* TargetActor)
{
}

void AGNREffectActorBase::HandleEffectEndOverlap(AActor* TargetActor)
{
}

UAbilitySystemComponent* AGNREffectActorBase::GetTargetASC(AActor* TargetActor) const
{
	if (!IsValid(TargetActor))
	{
		return nullptr;
	}

	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
}

FActiveGameplayEffectHandle AGNREffectActorBase::ApplyEffectToTarget(AActor* TargetActor)
{
    if (!IsValid(TargetActor))
    {
        return FActiveGameplayEffectHandle();
    }

    if (!EffectClass)
    {
        return FActiveGameplayEffectHandle();
    }

    UAbilitySystemComponent* TargetASC = GetTargetASC(TargetActor);

    if (!TargetASC)
    {
        return FActiveGameplayEffectHandle();
    }

    FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(
        EffectClass,
        EffectLevel,
        EffectContext
    );

    if (!SpecHandle.IsValid())
    {
        return FActiveGameplayEffectHandle();
    }

    return TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AGNREffectActorBase::RemoveEffectFromTarget(
    AActor* TargetActor,
    FActiveGameplayEffectHandle EffectHandle
)
{
    if (!IsValid(TargetActor))
    {
        return;
    }

    if (!EffectHandle.IsValid())
    {
        return;
    }

    UAbilitySystemComponent* TargetASC = GetTargetASC(TargetActor);

    if (!TargetASC)
    {
        return;
    }

    TargetASC->RemoveActiveGameplayEffect(EffectHandle);
}