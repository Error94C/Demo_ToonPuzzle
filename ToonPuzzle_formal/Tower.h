#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:

	ATower();

	virtual void Tick(float DeltaTime) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void HandleDestruction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	class UFloatingPawnMovement* MovementComponent;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 600.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float InvestigateRange = 1000.f;

	class ATank* Tank = nullptr;

	FTimerHandle FireRateTimerHandle;

	float FireRate = 2.f;

	void CheckFireCondition();

	bool InFireRange();

	void UpdateCurrentTargetTank();
};
