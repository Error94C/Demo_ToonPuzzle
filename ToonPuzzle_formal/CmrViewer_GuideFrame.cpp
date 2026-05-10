#include "CameraViewer.h"

void ACameraViewer::HideAllGuideFrames()
{
	for (const TPair<int32, FGuideFrameActor>& Pair : MapCaseToGuideFrame)
	{
		for (AActor* BorderActor : Pair.Value.GuideFrameActors)
		{
			SetGuideFrameVisiblity(BorderActor, false);
		}
	}
}

void ACameraViewer::SetGuideFrameShader(int32 CaseID)
{
	HideAllGuideFrames();

	const FGuideFrameActor* FoundGroup = MapCaseToGuideFrame.Find(CaseID);

	if (!FoundGroup) return;

	for (AActor* BorderActor : FoundGroup->GuideFrameActors)
	{
		SetGuideFrameVisiblity(BorderActor, true);
	}
}

void ACameraViewer::SetGuideFrameVisiblity(AActor* BorderActor, bool bVisible)
{
	if (!BorderActor) return;

	BorderActor->SetActorHiddenInGame(!bVisible);
	BorderActor->SetActorEnableCollision(bVisible);
}