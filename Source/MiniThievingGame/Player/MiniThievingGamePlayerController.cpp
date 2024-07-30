#include "MiniThievingGamePlayerController.h"

// Engine
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Project
#include "MiniThievingGame/Characters/MiniThievingGameCharacter.h"
#include "MiniThievingGame/Core/MiniThievingGameGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMiniThievingGamePlayerController::AMiniThievingGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AMiniThievingGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	const auto World = GetWorld();
	if (!IsValid(World)) return;

	if (const auto GM = UGameplayStatics::GetGameMode(World))
	{
		if (const auto MTGGameMode = Cast<AMiniThievingGameGameMode>(GM))
			SetPlayerWaiting(MTGGameMode->GetCurrentGamePhase() == EGamePhase::Waiting);
	}
}

void AMiniThievingGamePlayerController::SetPlayerWaiting(const bool bIsWaiting)
{
	if (bIsWaiting)
	{
		ShowStartScreen();
		SetMovementInputEnabled(false);
		bIsWaitingForInput = true;
	}
	else
	{
		HideStartScreen();
		SetMovementInputEnabled(true);
	}
}

void AMiniThievingGamePlayerController::PlayerGameOver(const bool bHasWon)
{
	ShowGameOverScreen(bHasWon);

	SetMovementInputEnabled(false);
}

void AMiniThievingGamePlayerController::OnAnyKeyPressed()
{
	if (bIsWaitingForInput)
	{
		const auto World = GetWorld();
		if (!IsValid(World)) return;

		bIsWaitingForInput = false;

		if (const auto GM = UGameplayStatics::GetGameMode(World))
		{
			if (const auto MTGGameMode = Cast<AMiniThievingGameGameMode>(GM))
				MTGGameMode->ChangeGamePhase(EGamePhase::InProgress);
		}
	}
}

void AMiniThievingGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AMiniThievingGamePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AMiniThievingGamePlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AMiniThievingGamePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AMiniThievingGamePlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMiniThievingGamePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMiniThievingGamePlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMiniThievingGamePlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMiniThievingGamePlayerController::OnTouchReleased);

		// Input Actions
		EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Triggered, this, &AMiniThievingGamePlayerController::OnCrouchTriggered);
		EnhancedInputComponent->BindAction(CameraRotationInputAction, ETriggerEvent::Started, this, &AMiniThievingGamePlayerController::OnRotateCameraInput);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMiniThievingGamePlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AMiniThievingGamePlayerController::OnSetDestinationTriggered()
{
	if (!bIsMovementInputEnabled) return;

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AMiniThievingGamePlayerController::OnSetDestinationReleased()
{
	if (!bIsMovementInputEnabled) return;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AMiniThievingGamePlayerController::OnTouchTriggered()
{
	if (!bIsMovementInputEnabled) return;

	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AMiniThievingGamePlayerController::OnTouchReleased()
{
	if (!bIsMovementInputEnabled) return;

	bIsTouch = false;
	OnSetDestinationReleased();
}

void AMiniThievingGamePlayerController::OnCrouchTriggered()
{
	if (!bIsMovementInputEnabled) return;

	if (const auto OwnedCharacter = GetCharacter())
	{
		if (!OwnedCharacter->bIsCrouched)
			OwnedCharacter->Crouch();
		else
			OwnedCharacter->UnCrouch();
	}
}

void AMiniThievingGamePlayerController::OnRotateCameraInput(const FInputActionInstance& Instance)
{
	if (!bIsMovementInputEnabled) return;

	const float FloatValue = Instance.GetValue().Get<float>();

	if (const auto ThiefCharacter = Cast<AMiniThievingGameCharacter>(GetCharacter()))
	{
		ThiefCharacter->RotateCameraBoom(FloatValue);
	}
}

void AMiniThievingGamePlayerController::ShowStartScreen()
{
	const auto World = GetWorld();
	if (!StartScreenWidget || !IsValid(World)) return;

	if (!IsValid(StartScreenInstance))
		StartScreenInstance = CreateWidget<UUserWidget>(World, StartScreenWidget);

	if (IsValid(StartScreenInstance))
		StartScreenInstance->AddToViewport(0);
}

void AMiniThievingGamePlayerController::HideStartScreen()
{
	if (IsValid(StartScreenInstance))
	{
		StartScreenInstance->RemoveFromParent();
		StartScreenInstance->Destruct();
	}
}

void AMiniThievingGamePlayerController::ShowGameOverScreen(const bool bHasWon)
{
	const auto World = GetWorld();
	if (!GameOverWidget || !IsValid(World)) return;

	if (!IsValid(GameOverInstance))
		GameOverInstance = CreateWidget<UUserWidget>(World, GameOverWidget);

	if (IsValid(GameOverInstance))
		GameOverInstance->AddToViewport(0);
}

void AMiniThievingGamePlayerController::RestartGame()
{
	const auto World = GetWorld();
	if (!IsValid(World)) return;

	UGameplayStatics::OpenLevel(this, FName(*World->GetName()), false);
}
