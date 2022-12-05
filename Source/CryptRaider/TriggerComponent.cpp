// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

}
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction)
{
    Super::TickComponent(_deltaTime, _tickType, _thisTickFunction);

    AActor* actor = GetAcceptableActor();
    if (actor != nullptr)
    {
        // Casting Actor
        UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor -> GetRootComponent());
        if(component != nullptr)
        {
            component -> SetSimulatePhysics(false);
        }
        actor -> AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

        mover -> SetShouldMove(true);
    }
    else
    {
        mover -> SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover* _mover)
{
    mover = _mover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> actors;
    GetOverlappingActors(actors);
    for (AActor* actor : actors)
    {
        bool isAcceptableTag = actor -> ActorHasTag(AcceptableActorTag);
        bool isGrabbed = actor -> ActorHasTag("Grabbed");
        if(isAcceptableTag && !isGrabbed)
        {
            return actor;
        }
    }
    return nullptr;
}