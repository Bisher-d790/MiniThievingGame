#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

// Project
#include "MiniThievingGame/AI/Guards/GuardSettings.h"

#include "MiniThievingGameAIController.generated.h"

class UMTGStateTreeComponent;
class UDataTable;

UCLASS()
class MINITHIEVINGGAME_API AMiniThievingGameAIController : public AAIController
{
	GENERATED_BODY()

#pragma region Override
public:
	AMiniThievingGameAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
#pragma endregion Override

#pragma region Components
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UMTGStateTreeComponent> StateTreeComponent = nullptr;
#pragma endregion Components

#pragma region AI
public:
	UPROPERTY()
	FGuardSettings GuardsSettings;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings")
	TObjectPtr<UDataTable> GuardsSettingsDA = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings")
	FName SettingsRowName = "Catcher";

	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings")
	FString SettingsRowContext = "Catcher";
#pragma endregion AI
};
