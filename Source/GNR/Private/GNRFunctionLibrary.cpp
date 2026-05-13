// Fill out your copyright notice in the Description page of Project Settings.


#include "GNRFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GNRAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Types/GNRCountDownAction.h"

UGNRAbilitySystemComponent* UGNRFunctionLibrary::NativeGetGNRASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UGNRAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UGNRFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UGNRAbilitySystemComponent* ASC = NativeGetGNRASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UGNRFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UGNRAbilitySystemComponent* ASC = NativeGetGNRASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UGNRFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UGNRAbilitySystemComponent* ASC = NativeGetGNRASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UGNRFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EGNRConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EGNRConfirmType::Yes : EGNRConfirmType::No;;
}

UPawnCombatComponent* UGNRFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	
	return nullptr;
}

UPawnCombatComponent* UGNRFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EGNRValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EGNRValidType::Valid : EGNRValidType::Invalid;

	return CombatComponent;
}

void UGNRFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EGNRCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EGNRCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
    UWorld* World = nullptr;

    if (GEngine)
    {
        World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    }

    if (!World)
    {
        return;
    }

    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

    FGNRCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FGNRCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

    if (CountDownInput == EGNRCountDownActionInput::Start)
    {
        if (!FoundAction)
        {
            LatentActionManager.AddNewAction(
                LatentInfo.CallbackTarget,
                LatentInfo.UUID,
                new FGNRCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
            );
        }
    }

    if (CountDownInput == EGNRCountDownActionInput::Cancel)
    {
        if (FoundAction)
        {
            FoundAction->CancelAction();
        }
    }
}

void UGNRFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EGNRInputMode InInputMode)
{
    APlayerController* PlayerController = nullptr;

    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            PlayerController = World->GetFirstPlayerController();
        }
    }

    if (!PlayerController)
    {
        return;
    }

    FInputModeGameOnly GameOnlyMode;
    FInputModeUIOnly UIOnlyMode;

    switch (InInputMode)
    {
    case EGNRInputMode::GameOnly:

        PlayerController->SetInputMode(GameOnlyMode);
        PlayerController->bShowMouseCursor = false;

        break;

    case EGNRInputMode::UIOnly:

        PlayerController->SetInputMode(UIOnlyMode);
        PlayerController->bShowMouseCursor = true;

        break;

    default:
        break;
    }
}
