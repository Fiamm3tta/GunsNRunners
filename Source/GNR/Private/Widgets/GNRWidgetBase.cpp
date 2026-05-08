// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GNRWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UGNRWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* UPlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponentInitialized(UPlayerUIComponent);
		}
	}
}