#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    // SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    // SpringArm->SetupAttachment(RootComponent);

    // Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
    PlayerInputComponent->BindAxis(TEXT("RotateTurret_AntiCursor"), this, &ATank::RotateTurretInput);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	
    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if (FMath::Abs(TurretInputValue) > KINDA_SMALL_NUMBER)
    {
        FRotator DeltaRotation = FRotator(0.f, TurretInputValue * TurretRotateSpeed * DeltaTime, 0.f);

        TurretMesh->AddLocalRotation(DeltaRotation);
    }
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;

    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    if (!bCanBaseTurn) return;

    FRotator DeltaRotation = FRotator::ZeroRotator;

    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}

void ATank::RotateTurretInput(float Value)
{
	TurretInputValue = Value;
}

void ATank::RotateTurretAxis(float Value)
{
	if (ATank* Tank = this)
	{
		Tank->RotateTurretInput(Value);
	}
}

void ATank::CanTankBaseTurn(bool bEnabled)
{
    bCanBaseTurn = bEnabled;
}