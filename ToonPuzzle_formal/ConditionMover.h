#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.h"
#include "ConditionMover.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UConditionMover : public UActorComponent
{
	GENERATED_BODY()

public:	

	UConditionMover();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* ConditionMover);

	UFUNCTION(BlueprintCallable)
	void IsCondition(AActor* KilledActor);

private:

	UPROPERTY(EditAnywhere)
	TArray<FName> TargetTags;

	UPROPERTY(EditAnywhere)
	int ConditionKilledCount = 2;

	int CurrentKilledCount = 0;

	UMover* MoverComponent;
};
