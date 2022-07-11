#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barricade.generated.h"

class AMovingLevel;

UCLASS()
class RUNNER_API ABarricade : public AActor
{
	GENERATED_BODY()
	
public:	
	ABarricade();
	void Init(AMovingLevel* Level);


	UFUNCTION(BlueprintCallable)
		AMovingLevel* GetLevel();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		AMovingLevel* Level_;
};
