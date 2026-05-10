#include "WallMover.h"

AWallMover::AWallMover()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
}

void AWallMover::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
}

void AWallMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform  (DeltaTime);
	RotatePlatform(DeltaTime);
}

void AWallMover::MovePlatform(float DeltaTime)
{

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += PlatformVelocity * DeltaTime;
	SetActorLocation(CurrentLocation);
	DistanceMoved =	FVector::Dist(StartLocation, CurrentLocation);

	if(ShouldPlatformReturn())
	{
		FString ActorName = GetName();
		float OverShoot = (DistanceMoved - MovedDistance);

		FVector MovedDirection = PlatformVelocity.GetSafeNormal();
		StartLocation += (MovedDistance * MovedDirection);
		SetActorLocation(StartLocation);
		PlatformVelocity = (-PlatformVelocity);
	}
}

void AWallMover::RotatePlatform(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation += (RotationVelocity * DeltaTime);
	SetActorRotation(CurrentRotation);
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AWallMover::ShouldPlatformReturn() const
{
	return DistanceMoved > MovedDistance;
}
