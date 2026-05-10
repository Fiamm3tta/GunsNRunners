// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GNREnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "AbilitySystem/GNRAttributeSet.h"

AGNREnemyCharacter::AGNREnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");

	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
}

UPawnCombatComponent* AGNREnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AGNREnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AGNREnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AGNREnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GNRAbilitySystemComponent || !GNRAttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC or AttributeSet is null on % s"), *GetNameSafe(this));
		return;
	}

	GNRAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UGNRAttributeSet::GetCurrentSpeedAttribute()
	).AddUObject(this, &AGNREnemyCharacter::OnCurrentSpeedChanged);

	ApplyCurrentSpeedToMovement(GNRAttributeSet->GetCurrentSpeed());
}

void AGNREnemyCharacter::OnCurrentSpeedChanged(const FOnAttributeChangeData& Data)
{
	const float ClampedSpeed = FMath::Clamp(
		Data.NewValue,
		GNRAttributeSet->GetMinSpeed(),
		GNRAttributeSet->GetMaxSpeed()
	);

	ApplyCurrentSpeedToMovement(ClampedSpeed);

	UE_LOG(LogTemp, Log, TEXT("CurrentSpeed changed: Old=%f New=%f"), Data.OldValue, Data.NewValue);
}

void AGNREnemyCharacter::ApplyCurrentSpeedToMovement(float NewSpeed)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = NewSpeed;

		UE_LOG(LogTemp, Log, TEXT("Applied MaxWalkSpeed: %f"), NewSpeed);
	}
}

void AGNREnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AGNREnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(GNRAbilitySystemComponent);

					// Debug::Print(TEXT("Enemy Start Up Data Loaded"), FColor::Green);
				}
			}
		)
	);
}