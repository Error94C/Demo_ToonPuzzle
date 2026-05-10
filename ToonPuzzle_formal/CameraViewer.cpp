#include "CameraViewer.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"

ACameraViewer::ACameraViewer()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Root);
	Camera->FieldOfView = CameraFOV;
}

void ACameraViewer::BeginPlay()
{
	Super::BeginPlay();

	InitCameraOrbitAroundRotateCenter();
    UpdateCameraAroundRotateCenter();

	HideAllGuideFrames();
	SetGuideFrameShader(1);

	bIsAutoRotating = false;
}

void ACameraViewer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAutoRotating) UpdateCameraTransitionByTick(DeltaTime);

	UpdateCameraAroundRotateCenter();

	CameraAngleStateCheck();
}

void ACameraViewer::ActivateCameraViewer()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	PC->SetViewTarget(this);
	
	SetAngleTriggerEnabled(true);
}

void ACameraViewer::SetPlayerControllerInputLocked(bool bLocked)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (bLocked)
	{
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
		PC->bShowMouseCursor = false;
	}
	else
	{
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		PC->bShowMouseCursor = true;
	}
}
