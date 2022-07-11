#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MovingLevel.h"
#include "MovingSegment.h"

#include "MovableActor.generated.h"

UCLASS()
class RUNNER_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AMovingSegment> SegmentClass_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int SegmentCount_;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	
	void Init(AMovingLevel* MovingLevel);
	void SpawnNextSegment();
	void DeleteFirstSegment();
	bool CheckFirstSegmentForDelete();
	bool CheckSegmentByDistance(int SegmentIndex, float Distance) const;
	//void SetMemorySize(int Size);

private:
	TArray<AMovingSegment*> Segments_;
	int MemorySize_;
	AMovingLevel* MovingLevel_;
};
