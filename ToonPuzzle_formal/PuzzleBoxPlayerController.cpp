#include "PuzzleBoxPlayerController.h"
#include "CameraViewer.h"
#include "Tank.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

APuzzleBoxPlayerController::APuzzleBoxPlayerController()
{
	bShowMouseCursor = true;

	bAutoManageActiveCameraTarget = false;
}

void APuzzleBoxPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CacheReferences();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	if (CameraViewer) SetViewTarget(CameraViewer);

	EnterModeBoxSideCamera();
}

void APuzzleBoxPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RotateCamera", IE_Pressed, this, &APuzzleBoxPlayerController::OnRotatePressed);
	InputComponent->BindAction("RotateCamera", IE_Released, this, &APuzzleBoxPlayerController::OnRotateReleased);
	InputComponent->BindAction("Reloadgame", IE_Pressed, this, &APuzzleBoxPlayerController::ReloadLevel);
	InputComponent->BindAction("SwitchController", IE_Pressed, this, &APuzzleBoxPlayerController::ToggleModeController);
}

void APuzzleBoxPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (CurrentMode != EPortalModeController::BoxSideCamera) return;
	if (!bIsRotatingCamera || !CameraViewer) return;

	float MouseX = 0.f;
	float MouseY = 0.f;
	GetInputMouseDelta(MouseX, MouseY);

	CameraViewer->CameraRotateControl(MouseX, MouseY, DeltaTime);
}

void APuzzleBoxPlayerController::CacheReferences()
{
	TankPawn = GetPawn();

	if (TankPawn)
	{
		TankHealthComponent = TankPawn->FindComponentByClass<UHealthComponent>();
	}

	CameraViewer = Cast<ACameraViewer>(UGameplayStatics::GetActorOfClass(this, ACameraViewer::StaticClass()));
}

void APuzzleBoxPlayerController::ReloadLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

float APuzzleBoxPlayerController::GetTankHealthPercent() const
{
	if (!TankHealthComponent) return 0.f;

	return TankHealthComponent->GetHealthPercent();
}

void APuzzleBoxPlayerController::ToggleModeController()
{
	if (CurrentMode == EPortalModeController::BoxSideCamera)
	{
		EnterModeBoxInsideScene();
	}
	else
	{
		EnterModeBoxSideCamera();
	}
}

void APuzzleBoxPlayerController::EnterModeBoxSideCamera()
{
	CurrentMode = EPortalModeController::BoxSideCamera;
	bIsRotatingCamera = false;

	CacheReferences();

	if (CameraViewer)
	{
		SetViewTarget(CameraViewer);
	}

	if (TankPawn)
	{
		TankPawn->DisableInput(this);
	}

	bShowMouseCursor = true;
}

void APuzzleBoxPlayerController::OnRotatePressed()
{
	if (CurrentMode == EPortalModeController::BoxSideCamera)
	{
		bIsRotatingCamera = true;
	}
}

void APuzzleBoxPlayerController::OnRotateReleased()
{
	bIsRotatingCamera = false;
}

void APuzzleBoxPlayerController::EnterModeBoxInsideScene()
{
	CurrentMode = EPortalModeController::BoxInsideScene;
	bIsRotatingCamera = false;

	CacheReferences();

	if (TankPawn)
	{
		Possess(TankPawn);
		TankPawn->EnableInput(this);
	}

	if (CameraViewer)
	{
		SetViewTarget(CameraViewer);
	}

	bShowMouseCursor = false;
}
