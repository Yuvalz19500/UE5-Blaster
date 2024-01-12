#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsCenter;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsLeft;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsRight;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsTop;

	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsBottom;
};


UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(TObjectPtr<UTexture2D> Texture, FVector2d ViewportCenter);

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& InHUDPackage) { HUDPackage = InHUDPackage; }
};
