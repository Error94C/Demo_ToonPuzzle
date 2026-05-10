#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallMover.generated.h"

UCLASS()
class TOONTANKS_API AWallMover : public AActor
{
	GENERATED_BODY()
	
public:	

	AWallMover();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category="Moving")
	FVector PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category="Moving")
	float MovedDistance = 100;

	UPROPERTY(VisibleAnywhere, Category="Moving")
	float DistanceMoved = -1;
	
	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category="Rotation")
	FRotator RotationVelocity;

	void MovePlatform(float DeltaTime);

	void RotatePlatform(float DeltaTime);
	
	bool ShouldPlatformReturn() const;

};
