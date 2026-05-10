#include "ToonTanksModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();

        if (ToonPuzzlePlayerController)
        {
            ToonPuzzlePlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
        ReloadGame();
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers--;

        if (TargetTowers == 0)
        {
            GameOver(true);
            if (ToonPuzzlePlayerController)
            {
                ToonPuzzlePlayerController->SetPlayerEnabledState(false);
            }
            ReloadGame();
        }
    }
}

void AToonTanksModeBase::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonPuzzlePlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonPuzzlePlayerController)
    {
        ToonPuzzlePlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonPuzzlePlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true
        );

        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

int AToonTanksModeBase::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(
        this,
        ATower::StaticClass(),
        Towers
    );
    return Towers.Num();
}

void AToonTanksModeBase::ReloadGame()
{
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &AToonTanksModeBase::ReloadLevel,
        StartDelay,   
        false   
    );
}

void AToonTanksModeBase::ReloadLevel()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}