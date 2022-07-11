#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Barricade.h"

#include "MovingSegment.generated.h"

class AMovingLine;

UCLASS()
class RUNNER_API AMovingSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingSegment();
	void Init(AMovingLine* Line);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (MakeEditWidget = true))
		FVector BarricadeSpawnLocation_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USceneComponent* RootSceneComp_;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
		FVector NextSegmentSpawnLocation_;

public:	
	void Cleanup();

	ABarricade* SpawnBarricade(TSubclassOf<ABarricade> BarricadeClass);
	void DestroyBarricade();

	UFUNCTION(BlueprintCallable)
		FVector GetNextSegmentSpawnLocation();
	UFUNCTION(BlueprintCallable)
		float GetSegmentSize(); // Temporary return float, will then return fvector;

private:
	UPROPERTY()
		AMovingLine* Line_;
	UPROPERTY()
		ABarricade* Barricade_;
};
