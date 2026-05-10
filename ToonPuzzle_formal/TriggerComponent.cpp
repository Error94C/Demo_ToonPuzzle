#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!MoverComponent) return;
    AActor* ActorIndex = GetAcceptableActor();
    if (ActorIndex)
    {
        MoverComponent->SetShouldMove(true);
    }
    else
    {
        MoverComponent->SetShouldMove(false);
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    AActor* ReturnActor = nullptr;
    
    TArray<AActor*> Actors;

    GetOverlappingActors(Actors);

    for (AActor* ActorIndex : Actors)
    {
        bool HasAcceptableActorTag = ActorIndex->ActorHasTag(AcceptableActorTag);

        if(HasAcceptableActorTag)
        {
            ReturnActor = ActorIndex;
        }
    }
    return ReturnActor;
} 

void UTriggerComponent::SetMover(UMover* NewMover)
{
    MoverComponent = NewMover;
}

void UTriggerComponent::StopMove()
{
    if (MoverComponent)
    {
        MoverComponent->SetShouldMove(false);
    }
}