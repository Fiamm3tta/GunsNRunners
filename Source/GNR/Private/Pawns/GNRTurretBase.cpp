// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/GNRTurretBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Projectile/GNRProjectile.h"
#include "Characters/GNRPlayerCharacter.h"

// Sets default values
AGNRTurretBase::AGNRTurretBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(SceneRoot);

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(BaseMesh);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(HeadMesh);

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(SceneRoot);

	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DetectionSphere->SetGenerateOverlapEvents(true);
	DetectionSphere->InitSphereRadius(DetectionRadius);

	AutoPossessAI = EAutoPossessAI::Disabled;
	CurrentHealth = MaxHealth;
}

void AGNRTurretBase::ApplyDamage(float DamageAmount)
{
	if (bIsDead)
	{
		return;
	}

	if (DamageAmount <= 0.f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}
}

// Called when the game starts or when spawned
void AGNRTurretBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	DetectionSphere->SetSphereRadius(DetectionRadius);

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleDetectionBeginOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleDetectionEndOverlap);
	DetectionSphere->UpdateOverlaps();

	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors, APawn::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if (!Pawn)
		{
			continue;
		}

		if (Pawn->IsPlayerControlled())
		{
			SetCurrentTarget(Pawn);
			break;
		}
	}

	StartFireLoop();
}

// Called every frame
void AGNRTurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead)
	{
		return;
	}
	UpdateAim(DeltaTime);
}

void AGNRTurretBase::HandleDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("BeginOverlap called: OtherActor = %s"), *GetNameSafe(OtherActor));

	if (bIsDead || !OtherActor)
	{
		return;
	}

	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (!OverlappedPawn)
	{
		// UE_LOG(LogTemp, Warning, TEXT("OtherActor is not a Pawn"));
		return;
	}

	// UE_LOG(LogTemp, Warning, TEXT("Detected Pawn: %s"), *GetNameSafe(OverlappedPawn));
	if (OverlappedPawn->IsPlayerControlled())
	{
		SetCurrentTarget(OverlappedPawn);
	}
}

void AGNRTurretBase::HandleDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (!OverlappedPawn)
	{
		return;
	}

	ClearCurrentTarget(OverlappedPawn);
}

void AGNRTurretBase::SetCurrentTarget(APawn* NewTarget)
{
	if (bIsDead)
	{
		return;
	}

	CurrentTarget = NewTarget;
}

void AGNRTurretBase::ClearCurrentTarget(APawn* TargetToClear)
{
	if (CurrentTarget == TargetToClear)
	{
		CurrentTarget = nullptr;
	}
}

void AGNRTurretBase::UpdateAim(float DeltaTime)
{
	if (!HasValidTarget())
	{
		return;
	}

	const FRotator CurrentRotation = HeadMesh->GetComponentRotation();
	const FRotator DesiredRotation = GetDesiredAimRotation();

	const FRotator NewRotation = FMath::RInterpConstantTo(
		CurrentRotation,
		DesiredRotation,
		DeltaTime,
		TurnSpeedDegPerSec);
	
	HeadMesh->SetWorldRotation(NewRotation);
}

FVector AGNRTurretBase::GetTargetAimLocation() const
{
	if (!CurrentTarget)
	{
		return FVector::ZeroVector;
	}

	return CurrentTarget->GetActorLocation();
}

FRotator AGNRTurretBase::GetDesiredAimRotation() const
{
	if (!CurrentTarget)
	{
		return HeadMesh->GetComponentRotation();
	}

	const FVector Start = HeadMesh->GetComponentLocation();
	const FVector Target = GetTargetAimLocation();

	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Roll = 0.f;

	return DesiredRotation;
}

bool AGNRTurretBase::HasValidTarget() const
{
	// UE_LOG(LogTemp, Warning, TEXT("CurretTarget is %s"), *GetNameSafe(CurrentTarget));
	
	return IsValid(CurrentTarget) && !bIsDead;
}

bool AGNRTurretBase::HasLineOfSightToTarget() const
{
	if (!HasValidTarget())
	{
		return false;
	}

	FHitResult HitResult;
	const FVector Start = MuzzlePoint->GetComponentLocation();
	const FVector End = GetTargetAimLocation();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(TurretLOS), false, this);
	Params.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params);
	/*
	const FVector DebugEnd = bHit ? HitResult.ImpactPoint : End;
	const FColor LineColor = bHit ? FColor::Red : FColor::Green;
	
	DrawDebugLine(
		GetWorld(),
		Start,
		DebugEnd,
		LineColor,
		false,
		1.0f,
		0,
		2.0f);

	DrawDebugLine(
		GetWorld(),
		DebugEnd,
		End,
		FColor::Blue,
		false,
		1.0f,
		0,
		1.0f);

	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			12.0f,
			12,
			FColor::Yellow,
			false,
			1.0f);

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[TurretLOS] HitActor=%s, HitComp=%s, Target=%s, Start=%s, End=%s"),
			*GetNameSafe(HitResult.GetActor()),
			*GetNameSafe(HitResult.GetComponent()),
			*GetNameSafe(CurrentTarget),
			*Start.ToCompactString(),
			*End.ToCompactString());
	}
	else
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[TurretLOS] No hit. Target=%s, Start=%s, End=%s"),
			*GetNameSafe(CurrentTarget),
			*Start.ToCompactString(),
			*End.ToCompactString());
	}

	if (!bHit)
	{
		return false;
	}

	const bool bHasLOS = (HitResult.GetActor() == CurrentTarget);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[TurretLOS] Result=%s"),
		bHasLOS ? TEXT("TRUE") : TEXT("FALSE"));
	*/
	return HitResult.GetActor() == CurrentTarget;
}

bool AGNRTurretBase::IsAimAligned() const
{
	if (!HasValidTarget())
	{
		return false;
	}

	const FRotator CurrentRotation = HeadMesh->GetComponentRotation();
	const FRotator DesiredRotation = GetDesiredAimRotation();

	const float YawDelta = FMath::Abs(
		FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, DesiredRotation.Yaw));

	return YawDelta <= MaxYawErrorToFire;
}

bool AGNRTurretBase::CanFire() const
{
	if (bIsDead)
	{
		return false;
	}

	if (!HasValidTarget())
	{
		return false;
	}

	if (!ProjectileClass)
	{
		return false;
	}
	
	if (!HasLineOfSightToTarget())
	{
		return false;
	}
	
	if (!IsAimAligned())
	{
		return false;
	}

	return true;
}

void AGNRTurretBase::StartFireLoop()
{
	if (FireInterval <= 0.f)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ThisClass::TryFire,
		FireInterval,
		true);
}

void AGNRTurretBase::StopFireLoop()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AGNRTurretBase::TryFire()
{
	if (!CanFire())
	{
		// UE_LOG(LogTemp, Warning, TEXT("I cannot fire..."));
		return;
	}

	FireProjectile();
}

void AGNRTurretBase::FireProjectile()
{
	// UE_LOG(LogTemp, Warning, TEXT("Fire!!!"));
	if (!ProjectileClass)
	{
		return;
	}

	const FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
	const FRotator SpawnRotation = MuzzlePoint->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AGNRProjectile>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams);

}

void AGNRTurretBase::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	AGNRPlayerCharacter* Player = Cast<AGNRPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	Player->HandleTurretKilled();

	OnTurretDied.Broadcast(this);

	// 일단 파괴 이후 연출 추가 예정
	Destroy();
}

