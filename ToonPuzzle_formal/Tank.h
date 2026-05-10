#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:

	ATank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 45.f;

public:

	void Move(float Value);
	
	void Turn(float Value);
	
	APlayerController* TankPlayerController;

public:

	void RotateTurretAxis(float Value);

	void RotateTurretInput(float Value);

private:

	float TurretInputValue = 0.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float TurretRotateSpeed = 90.f;

public:

	void CanTankBaseTurn(bool bEnabled);

private:

	bool bCanBaseTurn = false;
};
