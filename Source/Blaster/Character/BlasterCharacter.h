#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;
class AWeapon;
class UCombatComponent;
class UAnimMontage;
class ABlasterPlayerController;
class AController;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateHUDHealth();
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming) const;
	void PlayEliminatedMontage() const;
	virtual void OnRep_ReplicatedMovement() override;

	UFUNCTION(NetMulticast, Reliable)
	void Eliminated();

protected:
	virtual void BeginPlay() override;
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleJumpAction();
	void HandleEquipAction();
	void HandleCrouchAction();
	void HandleStartAimAction();
	void HandleCompleteAimAction();
	void HandleTriggerFireAction();
	void HandleCompleteFireAction();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	virtual void Jump() override;
	void PlayHitReactMontage() const;
	void CalculateAO_Pitch();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                   AController* InstigatorController, AActor* DamageCauser);

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Movement")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionLook;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionMove;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionJump;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionCrouch;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Combat")
	TObjectPtr<UInputAction> ActionEquip;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Combat")
	TObjectPtr<UInputAction> ActionAim;

	UPROPERTY(EditAnywhere, Category ="Player Input|Character Combat")
	TObjectPtr<UInputAction> ActionFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(const AWeapon* LastWeapon) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> Combat;

	UFUNCTION(Server, Reliable)
	void ServerEquipAction();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> EliminatedMontage;

	void HideCameraIfCharacterClose() const;

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed() const;

	/**
	* Player Health
	*/

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	TObjectPtr<ABlasterPlayerController> BlasterPlayerController;

	bool bEliminated = false;

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; };
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; };
	AWeapon* GetEquippedWeapon() const;
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; };
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; };
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; };
	FORCEINLINE bool IsEliminated() const { return bEliminated; };
};
