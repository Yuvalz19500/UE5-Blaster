#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

class UTexture2D;
class UCharacterOverlay;
class UUserWidget;

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

	UPROPERTY()
	float CrosshairSpread;

	UPROPERTY()
	FLinearColor CrosshairColor;
};


UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category="Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	TObjectPtr<UCharacterOverlay> CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();

private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(TObjectPtr<UTexture2D> Texture, FVector2d ViewportCenter, FVector2D Spread,
	                   FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& InHUDPackage) { HUDPackage = InHUDPackage; }
};
