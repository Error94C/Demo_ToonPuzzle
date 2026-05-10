#pragma once

#include "CameraAngleTriggerInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConditionTriggerManager.generated.h"

class UBoxComponent;

UCLASS()
class TOONTANKS_API AConditionTriggerManager : public AActor, public ICameraAngleTriggerInterface
{
	GENERATED_BODY()
	
public:	

	AConditionTriggerManager();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Case Condition")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Case Condition")
	FName AcceptableActorTag = TEXT("Player");

public:

	virtual bool CanExecuteCase_Implementation() override;

private:

	AActor* GetAcceptableActor() const;
};
