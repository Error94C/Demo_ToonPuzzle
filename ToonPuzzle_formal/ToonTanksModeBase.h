#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksModeBase.generated.h"

UCLASS()
class TOONTANKS_API AToonTanksModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void ActorDied(AActor* DeadActor);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	void ReloadGame();
	
	void ReloadLevel();

private:

	class ATank* Tank;
	class AToonTanksPlayerController* ToonPuzzlePlayerController;


	float StartDelay = 3.f;

	void HandleGameStart();

	int TargetTowers = 0;
	int GetTargetTowerCount();

};
