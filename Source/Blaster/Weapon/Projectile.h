#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundCue;

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	void SpawnPlayerImpactParticles(const FHitResult& Hit);
	void SpawnImpactParticleBasedOnSurfaceType(const FHitResult& Hit);
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);

	EPhysicalSurface ExtractSurfaceType(const FHitResult& Hit) const;


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnImpactParticlesAndSoundOnHit(bool bIsCharacterHit, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Tracer;

	TObjectPtr<UParticleSystemComponent> TracerComponent;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UParticleSystem>> PlayerImpactParticles;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UParticleSystem>> StoneImpactParticles;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UParticleSystem>> MetalImpactParticles;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ImpactSound;
};
