// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GNRPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "GNRGameplayTags.h"
#include "Components/Input/GNRInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "Weapon/GNRWeaponBase.h"
#include "AbilitySystem/GNRAttributeSet.h"
#include "Components/UI/PlayerUIComponent.h"
#include "GameModes/GNRStageGameMode.h"

AGNRPlayerCharacter::AGNRPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

void AGNRPlayerCharacter::EquipWeapon(AGNRWeaponBase* NewWeapon)
{
	if (!NewWeapon)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Not NewWeapon"));
		return;
	}

	if (CurrentWeapon)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Weapon equip already"));
		return;
	}

	CurrentWeapon = NewWeapon;
	CurrentWeapon->OnEquipped(this);
	bHasWeapon = true;
	// UE_LOG(LogTemp, Warning, TEXT("EquipWeapon called"));
}

void AGNRPlayerCharacter::UnequipWeapon()
{
	if (!CurrentWeapon)
	{
		bHasWeapon = false;
		return;
	}

	CurrentWeapon->OnUnequipped();
	CurrentWeapon = nullptr;
	bHasWeapon = false;
}

void AGNRPlayerCharacter::StartZoom()
{
	bIsZooming = true;
	// UE_LOG(LogTemp, Display, TEXT("Zoom Start!"));
}

void AGNRPlayerCharacter::StopZoom()
{
	bIsZooming = false;
	// UE_LOG(LogTemp, Display, TEXT("Zoom End!"));
}

UPawnUIComponent* AGNRPlayerCharacter::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* AGNRPlayerCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

void AGNRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GNRAbilitySystemComponent || !GNRAttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC or AttributeSet is null on % s"), *GetNameSafe(this));
		return;
	}

	GNRAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UGNRAttributeSet::GetCurrentSpeedAttribute()
	).AddUObject(this, &AGNRPlayerCharacter::OnCurrentSpeedChanged);

	ApplyCurrentSpeedToMovement(GNRAttributeSet->GetCurrentSpeed());

}

void AGNRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FirstPersonCameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera is null"));
		return;
	}

	const float TargetFOV = bIsZooming ? ZoomFOV : DefaultFOV;

	const float NewFOV = FMath::FInterpTo(
		FirstPersonCameraComponent->FieldOfView,
		TargetFOV,
		DeltaTime,
		ZoomInterpSpeed
	);

	FirstPersonCameraComponent->SetFieldOfView(NewFOV);
}

void AGNRPlayerCharacter::OnCurrentSpeedChanged(const FOnAttributeChangeData& Data)
{
	const float ClampedSpeed = FMath::Clamp(
		Data.NewValue,
		GNRAttributeSet->GetMinSpeed(),
		GNRAttributeSet->GetMaxSpeed()
	);

	ApplyCurrentSpeedToMovement(ClampedSpeed);

	// UE_LOG(LogTemp, Log, TEXT("CurrentSpeed changed: Old=%f New=%f"), Data.OldValue, Data.NewValue);
}

void AGNRPlayerCharacter::ApplyCurrentSpeedToMovement(float NewSpeed)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = NewSpeed;

		GetPawnUIComponent()->OnCurrentSpeedChanged.Broadcast(GetGNRAttributeSet()->GetCurrentSpeed());

		// UE_LOG(LogTemp, Log, TEXT("Applied MaxWalkSpeed: %f"), NewSpeed);
	}
}

void AGNRPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			int32 ApplyLevel = 1;

			if (AGNRStageGameMode* StageGameMode = GetWorld()->GetAuthGameMode<AGNRStageGameMode>())
			{
				if (StageGameMode->GetbHard())
				{
					ApplyLevel = 2;
				}
			}

			LoadedData->GiveToAbilitySystemComponent(GNRAbilitySystemComponent, ApplyLevel);
		}
	}
}

void AGNRPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called"));
	// UE_LOG(LogTemp, Warning, TEXT("InputComponent class: %s"), *PlayerInputComponent->GetClass()->GetName());

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UGNRInputComponent* GNRInputComponent = CastChecked<UGNRInputComponent>(PlayerInputComponent);

	GNRInputComponent->BindNativeInputAction(InputConfigDataAsset, GNRGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	GNRInputComponent->BindNativeInputAction(InputConfigDataAsset, GNRGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	GNRInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AGNRPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGNRPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGNRPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	GNRAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AGNRPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	GNRAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AGNRPlayerCharacter::HandleTurretKilled()
{
	ApplyTurretKillRewardEffect();
}

void AGNRPlayerCharacter::ApplyTurretKillRewardEffect()
{
	if (!SpeedBuffEffectClass)
	{
		return;
	}
	UGNRAbilitySystemComponent* ASC = GetGNRAbilitySystemComponent();
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle =
		ASC->MakeOutgoingSpec(SpeedBuffEffectClass, 1.f, Context);

	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
