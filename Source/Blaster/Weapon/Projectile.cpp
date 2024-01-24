#include "Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Blaster.h"
#include "Net/UnrealNetwork.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox, NAME_None, GetActorLocation(),
		                                                         GetActorRotation(),
		                                                         EAttachLocation::KeepWorldPosition);
	}

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::SpawnPlayerImpactParticles(const FHitResult& Hit)
{
	checkf(PlayerImpactParticles.Num() > 0, TEXT("PlayerImpactParticles array is empty"));
	const int32 RandomIndex = FMath::RandRange(0, PlayerImpactParticles.Num() - 1);

	const FRotator ImpactRotation = FRotationMatrix::MakeFromZ(Hit.ImpactNormal).Rotator();
	const FVector ImpactScale = FVector(0.75f, 0.75f, 0.75f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PlayerImpactParticles[RandomIndex],
	                                         Hit.ImpactPoint,
	                                         ImpactRotation, ImpactScale);
}

EPhysicalSurface AProjectile::ExtractSurfaceType(const FHitResult& Hit) const
{
	EPhysicalSurface SurfaceTypeTest = SurfaceType_Default;

	if (Hit.GetComponent())
	{
		const UMaterialInterface* HitMaterial = Hit.GetComponent()->GetMaterial(0);
		if (HitMaterial)
		{
			const UPhysicalMaterial* PhysMaterial = HitMaterial->GetPhysicalMaterial();
			if (PhysMaterial)
			{
				SurfaceTypeTest = PhysMaterial->SurfaceType;
			}
		}
	}

	return SurfaceTypeTest;
}

void AProjectile::SpawnImpactParticleBasedOnSurfaceType(const FHitResult& Hit)
{
	const EPhysicalSurface SurfaceType = ExtractSurfaceType(Hit);
	UParticleSystem* ImpactParticle = nullptr;

	switch (SurfaceType)
	{
	case EPS_Metal:
		{
			checkf(MetalImpactParticles.Num() > 0, TEXT("MetalImpactParticles array is empty"));
			const int32 RandomIndex = FMath::RandRange(0, MetalImpactParticles.Num() - 1);
			ImpactParticle = MetalImpactParticles[RandomIndex];
			break;
		}
	case EPS_Stone:
		{
			checkf(StoneImpactParticles.Num() > 0, TEXT("StoneImpactParticles array is empty"));
			const int32 RandomIndex = FMath::RandRange(0, StoneImpactParticles.Num() - 1);
			ImpactParticle = StoneImpactParticles[RandomIndex];
			break;
		}
	default: break;
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle,
		                                         Hit.ImpactPoint);
	}
}

void AProjectile::MulticastSpawnImpactParticlesAndSoundOnHit_Implementation(
	const bool bIsCharacterHit, const FHitResult& Hit)
{
	if (bIsCharacterHit)
	{
		SpawnPlayerImpactParticles(Hit);
	}
	else
	{
		SpawnImpactParticleBasedOnSurfaceType(Hit);
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	const ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	MulticastSpawnImpactParticlesAndSoundOnHit(BlasterCharacter ? true : false, Hit);

	Destroy();
}
