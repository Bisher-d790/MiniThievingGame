#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MiniThievingGameGameMode.generated.h"

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
	void StartPlay();

#pragma region GamePhases
public:
	UFUNCTION(BlueprintCallable, Category = "GameMode|GamePhase")
	void ChangeGamePhase(EGamePhase NewGamePhase);

	UFUNCTION(BlueprintPure, Category = "GameMode|GamePhase")
	EGamePhase GetCurrentGamePhase() const { return CurrentGamePhase; };

protected:
	EGamePhase CurrentGamePhase = EGamePhase::Waiting;

	virtual void StartWaitingPhase() {};
	virtual void StartInProgressPhase();
	virtual void StartFinishedPhase() {};
#pragma endregion GamePhases
};



