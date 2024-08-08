#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"

#include "MTGStateTreeComponent.generated.h"


UCLASS()
class MINITHIEVINGGAME_API UMTGStateTreeComponent : public UStateTreeComponent
{
	GENERATED_BODY()

public:
	inline void SetStateTreeRef(UStateTree* Tree)
	{
		StateTreeRef = FStateTreeReference();

		StateTreeRef.SetStateTree(Tree);
	}
};
