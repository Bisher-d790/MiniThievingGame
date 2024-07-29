#include "MiniThievingGameGameMode.h"

// Engine
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Project
#include "MiniThievingGamePlayerController.h"
#include "MiniThievingGameCharacter.h"

void AMiniThievingGameGameMode::StartPlay()
{
	Super::StartPlay();

	ChangeGamePhase(EGamePhase::Waiting);
}

void AMiniThievingGameGameMode::ChangeGamePhase(EGamePhase NewGamePhase)
{
	CurrentGamePhase = NewGamePhase;

	switch (NewGamePhase)
	{
	case EGamePhase::Waiting:
		StartWaitingPhase();
		break;

	case EGamePhase::InProgress:
		StartInProgressPhase();
		break;

	case EGamePhase::Finished:
		StartFinishedPhase();
		break;
	}
}

void AMiniThievingGameGameMode::StartInProgressPhase()
{
	const auto World = GetWorld();
	if (!IsValid(World)) return;

	const auto PlayerControllerNum = UGameplayStatics::GetNumPlayerControllers(World);
	for (int PCIndex = 0; PCIndex < PlayerControllerNum; PCIndex++)
	{
		if (const auto PC = UGameplayStatics::GetPlayerController(World, PCIndex))
			if (const auto MTGPC = Cast<AMiniThievingGamePlayerController>(PC))
				MTGPC->SetPlayerWaiting(false);
	}
}
