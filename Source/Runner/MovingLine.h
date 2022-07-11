#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MovingSegment.h"
#include "Barricade.h"

#include "MovingLine.generated.h"

class AMovingLevel;

UCLASS()
class RUNNER_API AMovingLine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingLine();
	void Init(int MemorySize, AMovingLevel* Level);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AMovingSegment> MovingSegmentClass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (MakeEditWidget = true))
		FVector LeftLineSpawnLocation_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (MakeEditWidget = true))
		FVector RightLineSpawnLocation_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (MakeEditWidget = true))
		FVector CharacterStandLocation_;
public:	
	UFUNCTION(BlueprintCallable)
		FVector GetLeftLineSpawnLocation();
	UFUNCTION(BlueprintCallable)
		FVector GetRightLineSpawnLocation();
	UFUNCTION(BlueprintCallable)
		FVector GetCharacterStandLocation();
	UFUNCTION(BlueprintCallable)
		AMovingLine* GetLeftLine();
	UFUNCTION(BlueprintCallable)
		AMovingLine* GetRightLine();
	UFUNCTION(BlueprintCallable)
		TArray<AMovingSegment*>& GetSegments();

	UFUNCTION(BlueprintCallable)
		void SetLeftLine(AMovingLine* Line);
	UFUNCTION(BlueprintCallable)
		void SetRightLine(AMovingLine* Line);

	UFUNCTION(BlueprintCallable)
		AMovingLevel* GetLevel();


	void SpawnNextSegment(TSubclassOf<ABarricade> BarricadeClass = NULL);
	void DeleteFirstSegment();
	bool CheckFirstSegmentForDelete();
	bool CheckSegmentByDistance(int SegmentIndex, float Distance) const;
	void SetMemorySize(int Size);

private:
	TArray<AMovingSegment*> Segments_;
	TArray<ABarricade*> Barricades_;

	int MemorySize_;

	UPROPERTY()
		AMovingLine* LeftLine_;
	UPROPERTY()
		AMovingLine* RightLine_;
	UPROPERTY()
		AMovingLevel* Level_;
};
