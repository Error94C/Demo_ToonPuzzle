#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit
	)
	{
		auto MyOwner = GetOwner();
		if (MyOwner == nullptr)
		{
			Destroy();
			return;
		}

		AController* MyOwnerInsitigator = MyOwner->GetInstigatorController();
		UClass* DamageTypeClass = UDamageType::StaticClass();

		if (OtherActor && OtherActor != this && OtherActor != MyOwner)
		{
			UGameplayStatics::ApplyDamage(
				OtherActor, 
				Damage, 
				MyOwnerInsitigator, 
				this, 
				DamageTypeClass
			);

			if (HitParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					this,
					HitParticles,
					GetActorLocation(),
					GetActorRotation()
				);
			}

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
			}
			
			if (HitCameraShakeClass)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
			}
		}
		Destroy();
	}
