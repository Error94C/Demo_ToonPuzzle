#include "TowerAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATowerAIController::BeginPlay()
{
    Super::BeginPlay();
    
    if (AIBehavior)
    {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        GetBlackboardComponent()->SetValueAsVector(TEXT("EnemyStartLocation"), GetPawn()->GetActorLocation());
    }
}

void ATowerAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}