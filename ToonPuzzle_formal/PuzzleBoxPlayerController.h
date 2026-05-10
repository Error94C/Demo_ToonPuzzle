#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PuzzleBoxPlayerController.generated.h"

class UUserWidget;
class ACameraViewer;
class UHealthComponent;

UENUM(BlueprintType)
enum class EPortalModeController : uint8
{
	BoxSideCamera,
	BoxInsideScene
};

UCLASS()
class TOONTANKS_API APuzzleBoxPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APuzzleBoxPlayerController();

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

	void ToggleModeController();

	void EnterModeBoxSideCamera();

	void EnterModeBoxInsideScene();

	float GetTankHealthPercent() const;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HUD = nullptr;

	UPROPERTY()
	APawn* TankPawn = nullptr;

	UPROPERTY()
	ACameraViewer* CameraViewer = nullptr;

	UPROPERTY()
	UHealthComponent* TankHealthComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Control")
	EPortalModeController CurrentMode = EPortalModeController::BoxSideCamera;

	bool bIsRotatingCamera = false;

	void OnRotatePressed();

	void OnRotateReleased();

	void ReloadLevel();
	
	void CacheReferences();
};
