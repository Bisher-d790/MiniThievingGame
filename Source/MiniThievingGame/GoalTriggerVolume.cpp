#include "GoalTriggerVolume.h"

// Engine
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Project
#include "MiniThievingGameCharacter.h"
#include "MiniThievingGameGameMode.h"

AGoalTriggerVolume::AGoalTriggerVolume()
{
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	SetRootComponent(Cast<USceneComponent>(Collision));
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AGoalTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Collision))
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	}
}

void AGoalTriggerVolume::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO Make it so only the artifacts (objective actors) are counted
	if (IsValid(OtherActor) && Cast<AMiniThievingGameCharacter>(OtherActor))
	{
		GoalReached(OtherActor, Cast<APawn>(OtherActor), 1);
	}
}

void AGoalTriggerVolume::GoalReached(const AActor* ObjectiveActor, const APawn* ScoringPawn, const int Points)
{
	const auto World = GetWorld();
	if (!IsValid(World) || !IsValid(ObjectiveActor) || !IsValid(ScoringPawn)) return;

	if (const auto GM = UGameplayStatics::GetGameMode(World))
		if (const auto MTGGameMode = Cast<AMiniThievingGameGameMode>(GM))
			MTGGameMode->PawnScored(ObjectiveActor, ScoringPawn, Points);
}
