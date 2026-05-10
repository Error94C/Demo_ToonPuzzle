#include "BTService_IfSeenPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

#include "NavigationSystem.h"

UBTService_IfSeenPlayerLocation::UBTService_IfSeenPlayerLocation()
{
    NodeName = "IfSeen Get Player Location";
}

void UBTService_IfSeenPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn)
    {
        if (OwnerComp.GetAIOwner())
        {
            if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
            }
        }
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    } 
}
