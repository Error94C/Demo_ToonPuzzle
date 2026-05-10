#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:

	virtual void BeginPlay() override;
	
public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BluePrintCallable)
	void SetMover(UMover* Mover);

	void StopMove();

private:

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UMover* MoverComponent;

	FTimerHandle MoveTimerHandle;

	AActor* GetAcceptableActor() const;
	
};
