#include "PickupComponent.h"

// Project
#include "MiniThievingGame/Objective/PickableActor.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupComponent::PickUp(APickableActor* Actor, USceneComponent* AttachToComp)
{
	if (IsValid(PickedUpActor))
		PickedUpActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (IsValid(Actor) && IsValid(AttachToComp))
		Actor->AttachToComponent(AttachToComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, PickUpSocket);

	PickedUpActor = Actor;
}
