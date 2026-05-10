#include "CameraViewer.h"
#include "Tank.h"
#include "TimerManager.h"
#include "BoxPuzzleStateManager.h"
#include "PuzzleBoxPlayerController.h"
#include "GameFramework/PlayerController.h"

void ACameraViewer::SwitchBoxEnterMode()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (APuzzleBoxPlayerController* PuzzlePC = Cast<APuzzleBoxPlayerController>(PC))
	{
		PuzzlePC->EnterModeBoxInsideScene();
	}
}

void ACameraViewer::ExecuteCaseTransition(int32 CaseID, int32 NextGuideFrameID, int32 NextStateIndex)
{
	SetPlayerControllerInputLocked(true);

	SetGuideFrameShader(NextGuideFrameID);

	if (PuzzleStateManager)
	{
		PuzzleStateManager->SwitchControledPawnByCase(CaseID, this);
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ATank* Tank = Cast<ATank>(PC->GetPawn()))
		{
			Tank->CanTankBaseTurn(CaseID == 2);
		}
	}

	if (SetCameraActorByCase(CaseID))
	{
		GetWorldTimerManager().SetTimer(
			CameraAngleSwitchTimerHandle,
			this,
			&ACameraViewer::StartCameraTransition,
			CameraAngleSwitchDelay,
			false
		);
	}
	else
	{
		SetPlayerControllerInputLocked(false);
	}

	BoxSceneStateIndex = NextStateIndex;
}

bool ACameraViewer::SetCameraActorByCase(int32 CaseID)
{
	AActor** FoundCamera = CaseToCameraMap.Find(CaseID);

	if (!FoundCamera || !(*FoundCamera)) return false;

	CameraAngleNextActor = *FoundCamera;

	return true;
}

void ACameraViewer::StartCameraTransition()
{
	StartCameraTransitionToActorLocation(CameraAngleNextActor);
}

void ACameraViewer::StartCameraTransitionToActorLocation(AActor* TargetActor)
{
	if (!TargetActor || !RotateTargetActor) 
	{
		SetPlayerControllerInputLocked(false);
		return;
	}

	const FVector Center = RotateTargetActor->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();

	const FVector ToCenter = Center - TargetLocation;
	const FRotator TargetLookRotation = ToCenter.Rotation();

	TransitionTargetYaw = TargetLookRotation.Yaw;
	TransitionTargetPitch = TargetLookRotation.Pitch;
	TransitionTargetDistance = FVector::Distance(TargetLocation, Center);

	bIsAutoRotating = true;
}

void ACameraViewer::UpdateCameraTransitionByTick(float DeltaTime)
{
	Yaw            = FMath::FixedTurn(Yaw, TransitionTargetYaw, AutoRotateSpeed * DeltaTime * 100.f);
	Pitch          = FMath::FInterpTo(Pitch, TransitionTargetPitch, DeltaTime, AutoRotateSpeed);
	RotateDistance = FMath::FInterpTo(RotateDistance, TransitionTargetDistance, DeltaTime, AutoRotateSpeed);

	const bool bYawDone      = FMath::Abs(FMath::FindDeltaAngleDegrees(Yaw, TransitionTargetYaw)) < 0.1f;
	const bool bPitchDone    = FMath::Abs(Pitch - TransitionTargetPitch) < 0.1f;
	const bool bDistanceDone = FMath::Abs(RotateDistance - TransitionTargetDistance) < 1.f;

	if (bYawDone && bPitchDone && bDistanceDone)
	{
		Yaw = TransitionTargetYaw;
		Pitch = TransitionTargetPitch;
		RotateDistance = TransitionTargetDistance;

		bIsAutoRotating = false;
		OnCameraTransitionFinished();
	}
}

void ACameraViewer::OnCameraTransitionFinished()
{
	if (bIsEndingSequencePending)
	{
		StartEndingSequence();
		return;
	}

	SetPlayerControllerInputLocked(false);
	SwitchBoxEnterMode();
}