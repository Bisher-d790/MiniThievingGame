#include "MiniThievingGameAIController.h"

// Engine
#include "MiniThievingGame/Components/MTGStateTreeComponent.h"

AMiniThievingGameAIController::AMiniThievingGameAIController() {
	StateTreeComponent = CreateDefaultSubobject<UMTGStateTreeComponent>(TEXT("StateTree"));
}

void AMiniThievingGameAIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GuardsSettingsDA))
	{
		if (const auto SettingsRow = GuardsSettingsDA->FindRow<FGuardSettings>(SettingsRowName, SettingsRowContext))
			GuardsSettings = *SettingsRow;
	}
}

void AMiniThievingGameAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	switch (GuardsSettings.Type)
	{
	case EGuardType::ST_Guard:
		if (IsValid(StateTreeComponent))
		{
			StateTreeComponent->SetStateTreeRef(GuardsSettings.StateTree);
			StateTreeComponent->StartLogic();
		}
		break;
	}
}
