#include "ConditionMover.h"

UConditionMover::UConditionMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConditionMover::BeginPlay()
{
	Super::BeginPlay();

	CurrentKilledCount = 0;
}

void UConditionMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UConditionMover::SetMover(UMover* ConditionMover)
{
	MoverComponent = ConditionMover;
}

void UConditionMover::IsCondition(AActor* KilledPawn)
{
	if (!KilledPawn || !MoverComponent) return;

	for (FName Tag : TargetTags)
	{
		if (KilledPawn->ActorHasTag(Tag))
		{
			CurrentKilledCount++;
			break;
		}
	}

	if (CurrentKilledCount >= ConditionKilledCount)
	{
		MoverComponent->SetShouldMove(true);
	}
}

