#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraAngleTriggerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCameraAngleTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class TOONTANKS_API ICameraAngleTriggerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanExecuteCase();

	virtual bool CanExecuteCase_Implementation();
};
