// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "BoxPuzzleStateManager.generated.h"

UCLASS()
class TOONTANKS_API ABoxPuzzleStateManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ABoxPuzzleStateManager();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pawn Switch")
	TMap<int32, APawn*> MapCaseToControledPawn;

	UFUNCTION(BlueprintCallable)
	bool SwitchControledPawnByCase(int32 CaseID, AActor* KeepViewTarget);
};
