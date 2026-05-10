#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TowerAIController.generated.h"

UCLASS()
class TOONTANKS_API ATowerAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UBehaviorTree* AIBehavior;
	
};
