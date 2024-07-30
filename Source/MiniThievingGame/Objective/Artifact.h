#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"

#include "Artifact.generated.h"

UCLASS()
class MINITHIEVINGGAME_API AArtifact : public APickableActor
{
	GENERATED_BODY()

#pragma region Points
public:
	UFUNCTION(BlueprintPure, Category = "Points")
	FORCEINLINE int GetPointsToGain() const { return PointsToGain; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Points")
	int PointsToGain = 1;
#pragma endregion Points
};
