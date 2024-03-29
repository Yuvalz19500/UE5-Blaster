#pragma once

#include "CoreMinimal.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

class ABlasterCharacter;
class ABlasterPlayerController;

UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PlayerEliminated(ABlasterCharacter* EliminatedCharacter,
	                              ABlasterPlayerController* EliminatedPlayerController,
	                              ABlasterPlayerController* AttackerPlayerController);
};
