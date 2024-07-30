#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MiniThievingGameCharacter.generated.h"

class APickableActor;

UCLASS(Blueprintable)
class AMiniThievingGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMiniThievingGameCharacter();

#pragma region Camera
public:
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = Camera)
	void RotateCameraBoom(const float RotateValue);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** The rotation degree to rotate the camera boom */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FRotator CameraRotation = FRotator(0.f, 0.f, 90.f);
#pragma endregion Camera

#pragma region PickUp
public:
	void PickUpActor(APickableActor* Actor);

	UFUNCTION(BlueprintPure, Category = "PickUp")
	FORCEINLINE APickableActor* GetPickedUpActor() const { return PickedUpActor; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	FName PickUpSocket = "HoldingSocket";

	UPROPERTY()
	APickableActor* PickedUpActor = nullptr;
#pragma endregion PickUp
};

