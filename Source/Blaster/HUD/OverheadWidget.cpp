// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveFromParent();
}

void UOverheadWidget::SetDisplayText(const FString& TextToDisplay) const
{
	DisplayText->SetText(FText::FromString(TextToDisplay));
}

void UOverheadWidget::ShowPlayerNetRole(const APawn* InPawn) const
{
	// const ENetRole RemoteRole = InPawn->GetRemoteRole();
	// FString Role;
	// switch (RemoteRole)
	// {
	// case ENetRole::ROLE_Authority:
	// 	Role = FString("Authority");
	// 	break;
	// case ENetRole::ROLE_AutonomousProxy:
	// 	Role = FString("Autonomous Proxy");
	// 	break;
	// case ENetRole::ROLE_SimulatedProxy:
	// 	Role = FString("Simulated Proxy");
	// 	break;
	// case ROLE_MAX:
	// 	Role = FString("Max");
	// 	break;
	// case ENetRole::ROLE_None:
	// 	Role = FString("None");
	// 	break;
	// }
	//
	// const FString RemoteRoleString = FString::Printf(TEXT("Local Role: %s"), *Role);

	if (const APlayerState* PawnState = InPawn->GetPlayerState())
	{
		SetDisplayText(PawnState->GetPlayerName());
	}
}
