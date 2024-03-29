#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UAnimationAsset;
class ACasing;
class UTexture2D;

UENUM(BlueprintType)
enum class EWeaponState: uint8
{
	EWS_Initial UMETA(DisplayName="Initial State"),
	EWS_Equipped UMETA(DisplayName="Equipped"),
	EWS_Dropped UMETA(DisplayName="Dropped"),

	EWS_MAX UMETA(DisplayName="DefaultMAX")
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void ShowPickupWidget(bool bShowWidget) const;
	virtual void Fire(const FVector& HitTarget);

	/**
	* Textures for the weapon crosshairs
	*/

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsRight;

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsTop;

	UPROPERTY(EditAnywhere, Category="Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsBottom;

	/**
	* Automatic fire
	*/

	UPROPERTY(EditAnywhere, Category="Combat")
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category="Combat")
	bool bAutomatic = true;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<UWidgetComponent> PickupWidget;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category="Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UAnimationAsset> FireAnimation;

	UFUNCTION()
	void OnRep_WeaponState() const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACasing> CasingClass;

	/**
	* Zoomed FOV while aiming
	*/
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

public:
	void SetWeaponState(const EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; };
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; };
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; };
	FORCEINLINE float GetZoomedInterpSpeed() const { return ZoomInterpSpeed; };
};
