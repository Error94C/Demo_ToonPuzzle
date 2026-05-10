#include "CameraViewer.h"
#include "Camera/CameraComponent.h"

void ACameraViewer::CameraRotateControl(float MouseX, float MouseY, float DeltaTime)
{
	if (bIsAutoRotating) return;

	Yaw   += MouseX * RotateSpeed * DeltaTime * 100.f;
	Pitch += MouseY * RotateSpeed * DeltaTime * 100.f;

	Pitch = FMath::Clamp(Pitch, MinPitch, MaxPitch);
}

void ACameraViewer::InitCameraOrbitAroundRotateCenter()
{
	if (!RotateTargetActor) return;

	const FVector Center = RotateTargetActor->GetActorLocation();
	const FVector CameraLocation = Camera->GetComponentLocation();

	const FVector ToCamera = CameraLocation - Center;

	RotateDistance = ToCamera.Size();

	const FVector ToCenter = Center - CameraLocation;
	const FRotator LookRot = ToCenter.Rotation();

	Yaw   = LookRot.Yaw;
	Pitch = LookRot.Pitch;
}

void ACameraViewer::UpdateCameraAroundRotateCenter()
{
	if (!RotateTargetActor) return;

	const FVector Center = RotateTargetActor->GetActorLocation();
	const FRotator OrbitRotation(Pitch, Yaw, 0.f);
	const FVector Offset = OrbitRotation.RotateVector(FVector(-RotateDistance, 0.f, 0.f));
	const FVector CameraLocation = Center + Offset;

	Camera->SetWorldLocation(CameraLocation);
	Camera->SetWorldRotation((Center - CameraLocation).Rotation());
}