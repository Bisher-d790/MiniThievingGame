#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MiniThievingGame/Interfaces/PickupInterface.h"

#include "MiniThievingGameCharacter.generated.h"

class APickableActor;
class UPickupComponent;

UCLASS(Blueprintable)
class AMiniThievingGameCharacter : public ACharacter, public IPickupInterface
{
	GENERATED_BODY()

#pragma region Override
public:
	AMiniThievingGameCharacter();

protected:
	/** Pickup Interface */
	virtual void PickUpActor(APickableActor* Actor) override;
	virtual UPickupComponent* GetPickupComponent() const override { return PickupComponent; };
#pragma endregion Override

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

#pragma region Components
protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<UPickupComponent> PickupComponent = nullptr;
#pragma endregion Components
};

