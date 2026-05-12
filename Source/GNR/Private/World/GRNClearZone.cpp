// Fill out your copyright notice in the Description page of Project Settings.


#include "World/GRNClearZone.h"
#include "Components/BoxComponent.h"

// Sets default values
AGRNClearZone::AGRNClearZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ClearnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ClearnBox"));
	SetRootComponent(ClearnBox);
	ClearnBox->InitBoxExtent(FVector(10.f, 10.f, 10.f));

	ClearnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ClearnBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ClearnBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ClearnBox->SetGenerateOverlapEvents(true);

	ClearnBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnEffectBeginOverlap);
}

void AGRNClearZone::OnEffectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BP_OnClearStage(OtherActor);
}

