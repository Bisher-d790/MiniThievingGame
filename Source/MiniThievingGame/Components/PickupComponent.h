#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PickupComponent.generated.h"

class APickableActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINITHIEVINGGAME_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPickupComponent();

public:
	void PickUp(APickableActor* Actor, USceneComponent* AttachToComp);

	UFUNCTION(BlueprintPure, Category = "PickUp")
	FORCEINLINE APickableActor* GetPickedUpActor() const { return PickedUpActor; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	FName PickUpSocket = "HoldingSocket";

	UPROPERTY()
	APickableActor* PickedUpActor = nullptr;
};
