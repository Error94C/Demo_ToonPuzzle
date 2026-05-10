#include "CameraViewer.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void ACameraViewer::StartEndingSequence()
{
	if (bIsEndingSequenceStarted) return;

	bIsEndingSequencePending = false;
	bIsEndingSequenceStarted = true;

	SetPlayerControllerInputLocked(true);

	if (!EndingSequence)
	{
		StartEndingWidget();
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
    
	PlaybackSettings.bAutoPlay = false;

	PlaybackSettings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;

	EndingSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		EndingSequence,
		PlaybackSettings,
		EndingSequenceActor
	);

	if (!EndingSequencePlayer)
	{
		StartEndingWidget();
		return;
	}

	EndingSequencePlayer->OnFinished.AddDynamic(this, &ACameraViewer::OnEndingSequenceFinished);
	EndingSequencePlayer->Play();
}

void ACameraViewer::OnEndingSequenceFinished()
{
	StartEndingWidget();
}

void ACameraViewer::StartEndingWidget()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (EndingWidgetClass)
	{
		EndingWidgetInstance = CreateWidget<UUserWidget>(PC, EndingWidgetClass);

		if (EndingWidgetInstance)
		{
			EndingWidgetInstance->AddToViewport();
		}
	}

	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);
	PC->bShowMouseCursor = true;

	FInputModeUIOnly InputMode;

	if (EndingWidgetInstance)
	{
		InputMode.SetWidgetToFocus(EndingWidgetInstance->TakeWidget());
	}

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
}