#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PickableActor.generated.h"

class USphereComponent;
class UMeshComponent;
class AMiniThievingGameCharacter;

UCLASS()
class MINITHIEVINGGAME_API APickableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickableActor();

	virtual void BeginPlay() override;

#pragma region Components
public:
	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE USphereComponent* GetCollision() const { return Collision; }

	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UMeshComponent* GetMesh() const { return Mesh; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USphereComponent> Collision = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UMeshComponent> Mesh = nullptr;
#pragma endregion Components

#pragma region PickUp
protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPickedUpByPlayer(AMiniThievingGameCharacter* Player);

	UFUNCTION(BlueprintPure, Category = "PickUp")
	FORCEINLINE AMiniThievingGameCharacter* GetPlayerPickedUpBy() const { return PlayerPickedUpBy; }

private:
	UPROPERTY() TObjectPtr<AMiniThievingGameCharacter> PlayerPickedUpBy = nullptr;
#pragma endregion PickUp
};
