#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "CameraViewer.generated.h"

class ABoxPuzzleStateManager;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UUserWidget;

USTRUCT(BlueprintType)
struct TOONTANKS_API FCameraAngleRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Angle Range")
	float TargetYaw = 0.f;

	UPROPERTY(EditAnywhere, Category="Angle Range")
	float TargetPitch = 0.f;

	UPROPERTY(EditAnywhere, Category="Angle Range")
	float ToleranceYaw = 5.f;

	UPROPERTY(EditAnywhere, Category="Angle Range")
	float TolerancePitch = 5.f;

	UPROPERTY(EditAnywhere, Category="Angle Range")
	bool bTriggered = false;

	UPROPERTY(EditAnywhere, Category="Angle Range")
	int32 CameraAngleStateIndex = 0;
};

USTRUCT(BlueprintType)
struct TOONTANKS_API FGuideFrameActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Guide")
	TArray<AActor*> GuideFrameActors;
};

UCLASS()
class TOONTANKS_API ACameraViewer : public APawn
{
	GENERATED_BODY()

// CameraViewer.cpp 部分声明
public:
	
	ACameraViewer();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ActivateCameraViewer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Puzzle Manager")
	ABoxPuzzleStateManager* PuzzleStateManager;

private:

	void SetPlayerControllerInputLocked(bool bLocked);

// CmrViewer_RotateOrbit.cpp 部分声明
public:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Box Window")
	AActor* RotateTargetActor = nullptr;

	UPROPERTY(EditAnywhere, Category="Box Window")
	float CameraFOV = 90.f;

	UPROPERTY(EditAnywhere, Category="Box Window")
	float RotateDistance = 1400.f;

	UPROPERTY(EditAnywhere, Category="Box Window")
	float RotateSpeed = 0.2f;

	UPROPERTY(EditAnywhere, Category="Box Window")
	float MinPitch = -80.f;

	UPROPERTY(EditAnywhere, Category="Box Window")
	float MaxPitch = 80.f;
	
	float Yaw = 0.f;

	float Pitch = 0.f;

	float GetOrbitYaw() const { return Yaw; }

    float GetOrbitPitch() const { return Pitch; }

	void CameraRotateControl(float MouseX, float MouseY, float DeltaTime);

private:

	void InitCameraOrbitAroundRotateCenter();

	void UpdateCameraAroundRotateCenter();

// CmrViewer_AngleTrigger.cpp 部分声明
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	bool bIsOnAngleTrigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	int32 BoxSceneStateIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	TArray<FCameraAngleRange> CameraAngleRangeTriggers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	AActor* ConditionActorCase1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	AActor* ConditionActorCase2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	AActor* ConditionActorCase3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Trigger")
	AActor* ConditionActorCase4 = nullptr;

	void SetAngleTriggerEnabled(bool bEnabled);

private:

	void CameraAngleStateCheck();

	bool CameraAngleCaseHandle(int32 TriggerID);

	bool CameraAngleCaseSwitch(int32 TriggerID) const;

	bool IsCameraAngleInRange(float CurrentYaw, float CurrentPitch, const FCameraAngleRange& Trigger) const;

	bool CanExecuteCaseCondition(AActor* InConditionActor);

// CmrViewer_Transition.cpp 部分声明
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Transition")
	TMap<int32, AActor*> CaseToCameraMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Transition")
	bool bIsAutoRotating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Transition")
	float AutoRotateSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Transition")
	float CameraAngleSwitchDelay = 1.f;

	float TransitionTargetYaw = 0.f;

	float TransitionTargetPitch = 0.f;

	float TransitionTargetDistance = 0.f;

private:

	FTimerHandle CameraAngleSwitchTimerHandle;

	AActor* CameraAngleNextActor = nullptr;

	void SwitchBoxEnterMode();

	void ExecuteCaseTransition(int32 CaseID, int32 NextGuideFrameID, int32 NextStateIndex);

	bool SetCameraActorByCase(int32 CaseID);

	void StartCameraTransition();

	void StartCameraTransitionToActorLocation(AActor* TargetActor);

	void UpdateCameraTransitionByTick(float DeltaTime);

	void OnCameraTransitionFinished();

// CmrViewer_GuideFrame.cpp 部分声明
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Angle Guide")
	TMap<int32, FGuideFrameActor> MapCaseToGuideFrame;

private:

	void HideAllGuideFrames();

	void SetGuideFrameShader(int32 CaseID);

	void SetGuideFrameVisiblity(AActor* BorderActor, bool bVisible);

// CmrViewer_EndGame.cpp 部分声明
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ending")
	ULevelSequence* EndingSequence = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ending")
	TSubclassOf<UUserWidget> EndingWidgetClass;

private:

	UPROPERTY()
	ULevelSequencePlayer* EndingSequencePlayer = nullptr;

	UPROPERTY()
	ALevelSequenceActor* EndingSequenceActor = nullptr;

	UPROPERTY()
	UUserWidget* EndingWidgetInstance = nullptr;

	bool bIsEndingSequencePending = false;

	bool bIsEndingSequenceStarted = false;

	void StartEndingSequence();

	void StartEndingWidget();

	UFUNCTION()
	void OnEndingSequenceFinished();

};
