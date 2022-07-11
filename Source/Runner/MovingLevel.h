#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MovingLine.h"
#include "BarricadeManager.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"

#include "MovingLevel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRollBackRowCurveTimelineFinished);

UCLASS()
class RUNNER_API AMovingLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingLevel();

	virtual void Tick(float DeltaTime) override;

	USceneComponent* MovingComp_;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AMovingLine> MovingLineClass_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FBarricadeInfo BarricadeInfo_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float Speed_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float SpeedMultiplier_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int RowCount_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int LineCount_;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int RowsMemorySize_;

	//RollBackRow Timline
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCurveFloat* RollBackRowCurve_;
	UPROPERTY(BlueprintReadOnly)
		UTimelineComponent* RollBackRowCurveTimeline_;
	UPROPERTY(BlueprintAssignable)
		FRollBackRowCurveTimelineFinished RollBackRowCurveTimelineFinished_;
protected:
	virtual void BeginPlay() override;

	virtual void SpawnLines();
	virtual void SpawnRow(int Count = 1, bool Empty = false);
	void DeleteSegmentRow();
	AMovingLine* SpawnLine(TSubclassOf<AMovingLine> LineClass, const FVector& Location, const FRotator& Rotation);

public:
	UFUNCTION(BlueprintCallable)
		void Init();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Start();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Stop();

	UFUNCTION(BlueprintCallable)
		AMovingLine* GetCurrentLine();
	UFUNCTION(BlueprintCallable)
		AMovingLine* MoveRight();
	UFUNCTION(BlueprintCallable)
		AMovingLine* MoveLeft();

	UFUNCTION(BlueprintCallable)
		void RollBackRow(const int RowCount);
	UFUNCTION()
		void RollBackRowProgress(float Value);

	UFUNCTION(BlueprintCallable)
		int GetMemorySize() const;
	UFUNCTION(BlueprintCallable)
		float GetScore() const;

	UFUNCTION(BlueprintCallable)
		void AddCoins(int Count);
	UFUNCTION(BlueprintCallable)
		int GetCoins();
	UFUNCTION(BlueprintCallable)
		void EmptyCoins();

private:
	bool CanMove_;
	int CurrentRowIndex_;
	float Score_;
	int Coins_;
	float RollBackDistance_;
	FVector RollBackDefaultPosition_;

	void MoveLevel();
	void DeleteBarricadeRow(int RowIndex);
	float CalculateRollBackDistance() const;

	UPROPERTY()
		AMovingLine* CurrentLine_;
	UPROPERTY()
		TArray<AMovingLine*> Lines_;
	UPROPERTY()
		UBarricadeManager* BarricadeManager_;


	UFUNCTION()
		void RollBackRowFinish();

	UFUNCTION()
		void InitMovingActors();
};
