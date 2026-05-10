#include "CameraViewer.h"
#include "CameraAngleTriggerInterface.h"

void ACameraViewer::SetAngleTriggerEnabled(bool bEnabled)
{
	bIsOnAngleTrigger = bEnabled;

	if (!bIsOnAngleTrigger)
	{
		for (FCameraAngleRange& Trigger : CameraAngleRangeTriggers)
		{
			Trigger.bTriggered = false;
		}
	}
}

bool ACameraViewer::IsCameraAngleInRange(float CurrentYaw, float CurrentPitch, const FCameraAngleRange& Trigger) const
{
	const float YawDiff   = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentYaw, Trigger.TargetYaw));
	const float PitchDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentPitch, Trigger.TargetPitch));

	return (YawDiff <= Trigger.ToleranceYaw) && (PitchDiff <= Trigger.TolerancePitch);
}

void ACameraViewer::CameraAngleStateCheck()
{
	if (!bIsOnAngleTrigger) return;

	for (FCameraAngleRange& Trigger : CameraAngleRangeTriggers)
	{
		if (Trigger.bTriggered) continue;

		if (!IsCameraAngleInRange(Yaw, Pitch, Trigger)) continue;

		if (!CameraAngleCaseSwitch(Trigger.CameraAngleStateIndex)) continue;

		const bool bExecuted = CameraAngleCaseHandle(Trigger.CameraAngleStateIndex);

		if (bExecuted) Trigger.bTriggered = true;

		return;
	}
}

bool ACameraViewer::CameraAngleCaseSwitch(int32 TriggerID) const
{
	switch (BoxSceneStateIndex)
	{
	case 0:
		return TriggerID == 1;

	case 1:
		return TriggerID == 2;

	case 2:
		return TriggerID == 3;

	case 3:
		return TriggerID == 4;

	default:
		return false;
	}
}

bool ACameraViewer::CameraAngleCaseHandle(int32 TriggerID)
{
	switch (TriggerID)
	{
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("Angle Trigger 1"));

		// 参数：CaseID，GuideFrame，BoxSceneStateIndex
		ExecuteCaseTransition(1, 2, 1);

		return true;

	case 2:
		UE_LOG(LogTemp, Warning, TEXT("Angle Trigger 2"));

		if (!CanExecuteCaseCondition(ConditionActorCase2)) return false;

		ExecuteCaseTransition(2, 3, 2);
		
		return true;

	case 3:
		UE_LOG(LogTemp, Warning, TEXT("Angle Trigger 3"));
		
		if (!CanExecuteCaseCondition(ConditionActorCase3)) return false;

		ExecuteCaseTransition(3, 4, 3);

		return true;

	case 4:
		UE_LOG(LogTemp, Warning, TEXT("Angle Trigger 4"));
		
		if (!CanExecuteCaseCondition(ConditionActorCase4)) return false;

		bIsEndingSequencePending = true;

		ExecuteCaseTransition(4, 5, 4);

		return true;

	default:
		
		return true;
	}
}

// 接口函数包装
bool ACameraViewer::CanExecuteCaseCondition(AActor* InConditionActor)
{
	if (!InConditionActor) return true;

	if (!InConditionActor->GetClass()->ImplementsInterface(UCameraAngleTriggerInterface::StaticClass())) return false;

	return ICameraAngleTriggerInterface::Execute_CanExecuteCase(InConditionActor);
}