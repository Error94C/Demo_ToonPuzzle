#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IfSeenPlayerLocation.generated.h"

UCLASS()
class TOONTANKS_API UBTService_IfSeenPlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTService_IfSeenPlayerLocation();
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
