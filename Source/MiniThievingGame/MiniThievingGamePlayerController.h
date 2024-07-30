#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"

#include "MiniThievingGamePlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AMiniThievingGamePlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region Override
public:
	AMiniThievingGamePlayerController();

	virtual void BeginPlay() override;
#pragma endregion Override

#pragma region Functionalities
public:
	virtual void SetPlayerWaiting(const bool bIsWaiting);

	virtual void PlayerGameOver(const bool bHasWon);
#pragma endregion Functionalities

#pragma region Input
public:
	UFUNCTION(BlueprintCallable, Category = Input)
	FORCEINLINE void SetMovementInputEnabled(const bool bIsEnabled) { bIsMovementInputEnabled = bIsEnabled; }

	UFUNCTION(BlueprintCallable, Category = Input)
	void OnAnyKeyPressed();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchInputAction;

	/** Camera Rotation Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraRotationInputAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	/** Input handlers for crouch. */
	void OnCrouchTriggered();
	/** Input handlers for rotating the camera. */
	void OnRotateCameraInput(const FInputActionInstance& Instance);

private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

	bool bIsMovementInputEnabled = true;

	bool bIsWaitingForInput = false;
#pragma endregion Input

#pragma region UI
public:
	void ShowStartScreen();

	void HideStartScreen();

	void ShowGameOverScreen(const bool bHasWon);

	UFUNCTION(BlueprintCallable, Category = UI)
	void RestartGame();

private:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> StartScreenWidget = nullptr;
	UPROPERTY() UUserWidget* StartScreenInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> GameOverWidget = nullptr;
	UPROPERTY() UUserWidget* GameOverInstance = nullptr;
#pragma endregion UI
};


