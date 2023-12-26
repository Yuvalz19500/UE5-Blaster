#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;
class ABlasterCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class BLASTER_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();
    friend ABlasterCharacter;
    void TickComponent(float DeltaTime, ELevelTick TickType,
                       FActorComponentTickFunction* ThisTickFunction) override;
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void EquipWeapon(AWeapon* WeaponToEquip);

protected:
    void BeginPlay() override;
    void SetAiming(bool bIsAiming);

    UFUNCTION(Server, Reliable)
    void ServerSetAiming(bool bIsAiming);

    UFUNCTION()
    void OnRep_EquippedWeapon() const;

private:
    UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
    TObjectPtr<AWeapon> EquippedWeapon;

    TObjectPtr<ABlasterCharacter> Character;

    UPROPERTY(Replicated)
    bool bAiming;

    UPROPERTY(EditAnywhere)
    float BaseWalkSpeed;

    UPROPERTY(EditAnywhere)
    float AimWalkSpeed;
};
