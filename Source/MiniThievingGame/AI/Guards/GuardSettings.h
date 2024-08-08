#pragma once

#include "CoreMinimal.h"

#include "GuardSettings.generated.h"

class UStateTree;

UENUM(BlueprintType)
enum class EGuardType : uint8 {
	ST_Guard,
	BT_Guard,
	SM_Guard
};

USTRUCT(BlueprintType, Blueprintable)
struct MINITHIEVINGGAME_API FGuardSettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Settings")
	EGuardType Type = EGuardType::ST_Guard;

	UPROPERTY(EditAnywhere, Category = "Settings|AI|StateTree", meta = (EditCondition = "Type == EGuardType::ST_Guard"))
	UStateTree* StateTree;
};