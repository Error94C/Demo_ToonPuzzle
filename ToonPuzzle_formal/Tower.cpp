#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TimerManager.h"

ATower::ATower()
{
    PrimaryActorTick.bCanEverTick = true;

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    
    if (RootComponent)
	{
		MovementComponent->SetUpdatedComponent(RootComponent);
	}
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

    if (MovementComponent && RootComponent)
    {
        MovementComponent->SetUpdatedComponent(RootComponent);
    }

    UpdateCurrentTargetTank();

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this,  &ATower::CheckFireCondition,  FireRate,  true);
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateCurrentTargetTank();

    if (Tank)
    {
        const float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        if (Distance <= FireRange)
        {
            RotateTurret(Tank->GetActorLocation()); 
        }
    }
}

UPawnMovementComponent* ATower::GetMovementComponent() const
{
	return MovementComponent;
}

void ATower::UpdateCurrentTargetTank()
{
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATower::CheckFireCondition()
{
    if (Tank)
    {
        if (InFireRange() && Tank->bAlive)
        {
            Fire();
        }
    }
}

bool ATower::InFireRange()
{
    if (Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true; 
        }
    }
    return false;
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    
    Destroy();
}
