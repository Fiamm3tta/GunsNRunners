// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GNRGameModeBase.h"
#include "GNRStageGameMode.generated.h"

class UGNRGameInstance;
class AGNRGameStateBase;

UENUM(BlueprintType)
enum class EGNRStageGameModeState : uint8
{
	PrepareGame,
	GameStart,
	PlayerDied,
	GameClear
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageGameModeStateChangedDelegate, EGNRStageGameModeState, CurrentState);

/**
 * 
 */
UCLASS()
class GNR_API AGNRStageGameMode : public AGNRGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleStageClear();

	UFUNCTION(BlueprintCallable)
	bool GetbHard() { return bHard; }

	UFUNCTION(BlueprintCallable)
	float GetResultTime() { return ResultTime; }

	UFUNCTION(BlueprintCallable)
	int32 GetResultStarCount() { return ResultStarCount; }
	
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UGNRGameInstance> CurrentGameInstance;

	UPROPERTY(Transient)
	TObjectPtr<AGNRGameStateBase> CurrentGameState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStageGameModeStateChangedDelegate OnStageGameModeStateChanged;

	UPROPERTY()
	EGNRStageGameModeState CurrentStageGameModeState;

	UPROPERTY()
	float TimePassedSinceStart = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrepareTime", meta = (AllowPrivateAccess = "true"))
	float PrepareTime = 5.f;

	void SetCurrentStageGameModeState(EGNRStageGameModeState InState);

	FName CurrentStageId = TEXT("Stage1");

	bool bHard = false;
	float ResultTime = 0;
	int32 ResultStarCount = 0;
};
