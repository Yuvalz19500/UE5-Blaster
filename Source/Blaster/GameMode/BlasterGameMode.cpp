#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* EliminatedCharacter,
                                        ABlasterPlayerController* EliminatedPlayerController,
                                        ABlasterPlayerController* AttackerPlayerController)
{
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Eliminated();
	}
}
