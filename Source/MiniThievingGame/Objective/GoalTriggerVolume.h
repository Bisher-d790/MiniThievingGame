#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GoalTriggerVolume.generated.h"

class UBoxComponent;

UCLASS()
class MINITHIEVINGGAME_API AGoalTriggerVolume : public AActor
{
	GENERATED_BODY()

public:
	AGoalTriggerVolume();

	virtual void BeginPlay() override;

#pragma region Components
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UBoxComponent> Collision = nullptr;
#pragma endregion Components

#pragma region Overlap
protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GoalReached(const AActor* ObjectiveActor, const APawn* ScoringPawn, const int Points);
#pragma endregion Overlap
};
