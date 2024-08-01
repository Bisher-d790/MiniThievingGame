#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PickupInterface.generated.h"

class APickableActor;
class UPickupComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class IPickupInterface
{
	GENERATED_BODY()

public:
	virtual void PickUpActor(APickableActor* Actor) {};

	virtual inline UPickupComponent* GetPickupComponent() const { return nullptr; };
};