// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	bUseSeamlessTravel = true;

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
