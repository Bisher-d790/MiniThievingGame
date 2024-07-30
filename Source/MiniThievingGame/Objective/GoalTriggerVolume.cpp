#include "GoalTriggerVolume.h"

// Engine
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Project
#include "MiniThievingGame/Core/MiniThievingGameGameMode.h"
#include "MiniThievingGame/Objective/Artifact.h"

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
	if (IsValid(OtherActor))
		if (const auto Artifact = Cast<AArtifact>(OtherActor))
			GoalReached(Artifact, Artifact->GetPointsToGain());
}

void AGoalTriggerVolume::GoalReached(const AArtifact* Artifact, const int Points)
{
	const auto World = GetWorld();
	if (!IsValid(World) || !IsValid(Artifact)) return;

	if (const auto GM = UGameplayStatics::GetGameMode(World))
		if (const auto MTGGameMode = Cast<AMiniThievingGameGameMode>(GM))
			MTGGameMode->PawnScored(Artifact, Points);
}
