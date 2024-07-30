#include "MiniThievingGame/Objective/PickableActor.h"

// Engine
#include "Components/SphereComponent.h"

// Project
#include "MiniThievingGame/Characters/MiniThievingGameCharacter.h"

// Sets default values
APickableActor::APickableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	SetRootComponent(Cast<USceneComponent>(Collision));
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->AttachToComponent(Collision, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetGenerateOverlapEvents(false);
}

void APickableActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Collision))
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	}
}

void APickableActor::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)) return;

	if (const auto MTGCharacter = Cast<AMiniThievingGameCharacter>(OtherActor))
	{
		OnPickedUpByPlayer(MTGCharacter);
	}
}

void APickableActor::OnPickedUpByPlayer(AMiniThievingGameCharacter* Player)
{
	if (!IsValid(Player)) return;

	Player->PickUpActor(this);

	PlayerPickedUpBy = Player;
}