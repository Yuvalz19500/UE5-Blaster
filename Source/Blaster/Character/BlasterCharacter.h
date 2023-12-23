#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;
class AWeapon;
class UCombatComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleJumpAction();
	void HandleEquipAction();
	void HandleCrouchAction();
	void HandleStartAimAction();
	void HandleCompleteAimAction();

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

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
};
