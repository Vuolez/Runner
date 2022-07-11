#include "MovingLevel.h"

#include "Kismet/GameplayStatics.h"
#include "MovableActor.h"

#include <cmath>

// Sets default values
AMovingLevel::AMovingLevel(){
	PrimaryActorTick.bCanEverTick = true;

	MovingComp_ = CreateDefaultSubobject<USceneComponent>(TEXT("MovingComp"));
	SetRootComponent(MovingComp_);

	RollBackRowCurveTimeline_ = CreateDefaultSubobject<UTimelineComponent>(TEXT("RollBackRowCurveTimelineComp"));

	CanMove_ = false;
}

void AMovingLevel::BeginPlay(){
	Super::BeginPlay();
}

void AMovingLevel::InitMovingActors() {
	TArray<AActor*> MovableActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMovableActor::StaticClass(), MovableActors);
	for (auto Actor : MovableActors) {
		AMovableActor*  MovableActor = Cast<AMovableActor>(Actor);
		if (MovableActor) {
			MovableActor->Init(this);
		}
	}
}


void AMovingLevel::Tick(float DeltaTime){

	if (Lines_.IsValidIndex(0)) {
		if (CanMove_) {
			MoveLevel();
		}

		if (Lines_[0]->CheckFirstSegmentForDelete()){
			DeleteSegmentRow();
			SpawnRow();
		}

		if (Lines_[0]->CheckSegmentByDistance(CurrentRowIndex_, -50)) {
			DeleteBarricadeRow(CurrentRowIndex_);
			CurrentRowIndex_ = (CurrentRowIndex_ + 1< RowsMemorySize_) ? ++CurrentRowIndex_ : CurrentRowIndex_;
		}
	}

	if (CanMove_) {
		Speed_ += SpeedMultiplier_;
		Score_ += Speed_ * DeltaTime;
	}

	Super::Tick(DeltaTime);
}


void AMovingLevel::MoveLevel() {
	MovingComp_->AddWorldOffset(FVector(1, 0, 0) * -Speed_);
}

void AMovingLevel::Init(){
	BarricadeManager_ = NewObject<UBarricadeManager>(this);
	BarricadeManager_->Init(LineCount_, BarricadeInfo_);

	SpawnLines();
	SpawnRow(RowsMemorySize_, true); // Spawn first empty lines
	SpawnRow(RowCount_);

	InitMovingActors();
}

void AMovingLevel::Start_Implementation() {
	CanMove_ = true;
}

void AMovingLevel::Stop_Implementation(){
	CanMove_ = false;
}

void AMovingLevel::SpawnLines() {
	AMovingLine* CurrentLine = nullptr;
	for (int i = 0; i < LineCount_; ++i) {
		if (!CurrentLine) {
			CurrentLine = SpawnLine(MovingLineClass_, GetActorLocation(), FRotator::ZeroRotator);
		}
		else {
			FVector Offset(0,0,0);
			Offset.Y = FVector::Distance(CurrentLine->GetActorLocation(), CurrentLine->GetRightLineSpawnLocation());
			AMovingLine* SpawnedLine = SpawnLine(MovingLineClass_, CurrentLine->GetRightLineSpawnLocation() + Offset, FRotator::ZeroRotator);

			CurrentLine->SetRightLine(SpawnedLine);
			SpawnedLine->SetLeftLine(CurrentLine);
			CurrentLine = SpawnedLine;
		}

		Lines_.Add(CurrentLine);
	}

	size_t MiddleIndex = std::ceil(Lines_.Num() / 2);
	CurrentLine_ = Lines_.IsValidIndex(MiddleIndex) ? Lines_[MiddleIndex] : nullptr;
}

void AMovingLevel::SpawnRow(int Count, bool Empty){
	for (int i = 0; i < Count; ++i) {
		if (!Empty) {
			TArray<TSubclassOf<ABarricade>> NextRow = BarricadeManager_->GetNexRow();
			for (int j = 0; j < Lines_.Num(); ++j) {
				Lines_[j]->SpawnNextSegment(NextRow[j]);
			}
		}
		else {
			for (int j = 0; j < Lines_.Num(); ++j) {
				Lines_[j]->SpawnNextSegment();
			}
		}
	}
}

void AMovingLevel::DeleteSegmentRow(){
	for (AMovingLine* Line : Lines_) {
		Line->DeleteFirstSegment();
	}
}

void AMovingLevel::DeleteBarricadeRow(int RowIndex) {
	for (AMovingLine* Line : Lines_) {
		if (Line->GetSegments().IsValidIndex(RowIndex)) {
			Line->GetSegments()[RowIndex]->DestroyBarricade();
		}
	}
}

void AMovingLevel::RollBackRow(const int RowCount){
	if (RollBackRowCurve_) {
		FOnTimelineFloat RollBackRowProgress;
		RollBackRowProgress.BindUFunction(this, FName("RollBackRowProgress"));
		RollBackRowCurveTimeline_->AddInterpFloat(RollBackRowCurve_, RollBackRowProgress);

		FOnTimelineEvent RollBackRowFinish;
		RollBackRowFinish.BindUFunction(this, FName("RollBackRowFinish"));
		RollBackRowCurveTimeline_->SetTimelineFinishedFunc(RollBackRowFinish);
	

		RollBackDistance_ = CalculateRollBackDistance();
		RollBackDefaultPosition_ = MovingComp_->GetComponentLocation();

		RollBackRowCurveTimeline_->PlayFromStart();

		CurrentRowIndex_ = 0;
	}
}

void AMovingLevel::RollBackRowProgress(float Value){
	FVector EndLocation = RollBackDefaultPosition_;
	EndLocation.X = RollBackDefaultPosition_.X + RollBackDistance_;
	MovingComp_->SetWorldLocation(FMath::Lerp(RollBackDefaultPosition_, EndLocation, Value));

}

int AMovingLevel::GetMemorySize() const{
	return RowsMemorySize_;
}

float AMovingLevel::GetScore() const{
	return Score_;
}

void AMovingLevel::RollBackRowFinish() {
	UE_LOG(LogTemp, Warning, TEXT("RollBackRowFinish"));
	RollBackRowCurveTimelineFinished_.Broadcast();
}

float AMovingLevel::CalculateRollBackDistance() const {
	if (Lines_.IsValidIndex(0)) {
		auto Segments = Lines_[0]->GetSegments();
		if (Segments.IsValidIndex(0)) {
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (PlayerPawn) {
				FVector SegmentLocation = Segments[0]->GetActorLocation();
				FVector PlayerLocation = FVector::ZeroVector;
				PlayerLocation.X = PlayerPawn->GetActorLocation().X;
				return FVector::Distance(SegmentLocation, PlayerLocation);
			}
		}
	}

	return 0.0f;
}

AMovingLine* AMovingLevel::SpawnLine(TSubclassOf<AMovingLine> LineClass, const FVector& Location, const FRotator& Rotation){
	if (LineClass) {
		FActorSpawnParameters SpawnInfo;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(LineClass, Location, Rotation, SpawnInfo);
		if (SpawnedActor) {
			AMovingLine* SpawnedLine = Cast<AMovingLine>(SpawnedActor);
			if (SpawnedLine) {
				SpawnedLine->Init(RowsMemorySize_, this);
				SpawnedLine->AttachToComponent(MovingComp_, FAttachmentTransformRules::KeepWorldTransform);
				return SpawnedLine;
			}
			return nullptr;
		}
		return nullptr;
	}

	return nullptr;
}



AMovingLine* AMovingLevel::GetCurrentLine(){
	return CurrentLine_;
}

AMovingLine* AMovingLevel::MoveRight() {
	AMovingLine* RightLine = nullptr;
	if (CurrentLine_) {
		RightLine = CurrentLine_->GetRightLine();
		CurrentLine_ = RightLine ? RightLine : CurrentLine_;
	}

	return RightLine;
}

AMovingLine* AMovingLevel::MoveLeft() {
	AMovingLine* LefttLine = nullptr;
	if (CurrentLine_) {
		LefttLine = CurrentLine_->GetLeftLine();
		CurrentLine_ = LefttLine ? LefttLine : CurrentLine_;
	}

	return LefttLine;
}


void AMovingLevel::AddCoins(int Count) {
	Coins_ += Count;
}

int AMovingLevel::GetCoins() {
	return Coins_;
}

void AMovingLevel::EmptyCoins() {
	Coins_ = 0;
}
