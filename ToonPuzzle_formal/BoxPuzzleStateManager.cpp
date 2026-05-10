#include "BoxPuzzleStateManager.h"
#include "BoxPuzzlePlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ABoxPuzzleStateManager::ABoxPuzzleStateManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoxPuzzleStateManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxPuzzleStateManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABoxPuzzleStateManager::SwitchControledPawnByCase(int32 CaseID, AActor* KeepViewTarget)
{
	APawn** FoundPawn = MapCaseToControledPawn.Find(CaseID);

	if (!FoundPawn || !(*FoundPawn)) return false;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!PC) return false;

	PC->Possess(*FoundPawn);

	if (ABoxPuzzlePlayerController* BoxPC = Cast<ABoxPuzzlePlayerController>(PC))
	{
		BoxPC->SetCameraControlEnabled(false);
	}

	if (KeepViewTarget) PC->SetViewTarget(KeepViewTarget);

	const bool bSuccess = (PC->GetPawn() == *FoundPawn);

	return bSuccess;
}