#include "ConditionTriggerManager.h"
#include "Components/BoxComponent.h"

AConditionTriggerManager::AConditionTriggerManager()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
}

void AConditionTriggerManager::BeginPlay()
{
	Super::BeginPlay();
}

void AConditionTriggerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AConditionTriggerManager::CanExecuteCase_Implementation()
{
	AActor* AcceptableActor = GetAcceptableActor();

	return AcceptableActor != nullptr;
}

AActor* AConditionTriggerManager::GetAcceptableActor() const
{
	if (!TriggerBox) return nullptr;

	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);

	for (AActor* ActorIndex : OverlappingActors)
	{
		if (!ActorIndex) continue;
		const bool bHasAcceptableTag = AcceptableActorTag.IsNone() || ActorIndex->ActorHasTag(AcceptableActorTag);
		if (bHasAcceptableTag) return ActorIndex;
	}

	return nullptr;
}
