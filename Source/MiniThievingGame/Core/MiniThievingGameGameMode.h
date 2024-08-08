#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MiniThievingGameGameMode.generated.h"

class AArtifact;

UENUM()
enum EGamePhase : uint8
{
	Waiting,
	InProgress,
	Finished
};

UCLASS(minimalapi)
class AMiniThievingGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

#pragma region GamePhases
public:
	UFUNCTION(BlueprintCallable, Category = "GameMode|GamePhase")
	void ChangeGamePhase(EGamePhase NewGamePhase);

	UFUNCTION(BlueprintPure, Category = "GameMode|GamePhase")
	FORCEINLINE EGamePhase GetCurrentGamePhase() const { return CurrentGamePhase; };

protected:
	EGamePhase CurrentGamePhase = EGamePhase::Waiting;

	virtual void StartWaitingPhase() {};
	virtual void StartInProgressPhase();
	virtual void StartFinishedPhase();
#pragma endregion GamePhases

#pragma region Scoring
public:
	UFUNCTION()
	void PawnScored(const AArtifact* Artifact, const int Points);

	UFUNCTION(BlueprintPure, Category = "Score")
	FORCEINLINE int GetScore() const { return Score; }

	inline void AddScore(const int AddedPoints) { Score += AddedPoints; }
	inline void ResetScore() { Score = 0; }

protected:
	inline virtual bool CheckScoreCondition(const AActor* ObjectiveActor) { return IsValid(ObjectiveActor); };
	inline virtual bool CheckGameEndCondition() { return Score >= TargetScore; };

private:
	UPROPERTY()
	int Score = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Score")
	int TargetScore = 1;
#pragma endregion Scoring
};



