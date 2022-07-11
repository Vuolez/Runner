#include "MovingLine.h"

//#include "DrawDebugHelpers.h"

AMovingLine::AMovingLine(){
	LeftLine_ = nullptr;
	RightLine_ = nullptr;
}

void AMovingLine::Init(int MemorySize, AMovingLevel* Level){
	Level_ = Level;
	MemorySize_ = MemorySize;
}

void AMovingLine::BeginPlay(){
	Super::BeginPlay();
}

FVector AMovingLine::GetLeftLineSpawnLocation(){
	return GetActorLocation() + LeftLineSpawnLocation_;
}

FVector AMovingLine::GetRightLineSpawnLocation(){
	return GetActorLocation() + RightLineSpawnLocation_;
}

void AMovingLine::SpawnNextSegment(TSubclassOf<ABarricade> BarricadeClass) {
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	FVector Location = Segments_.IsValidIndex(0) ? Segments_.Last()->GetNextSegmentSpawnLocation() : GetActorLocation();

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(MovingSegmentClass,Location, Rotation, SpawnInfo);
	if (SpawnedActor) {
		AMovingSegment* SpawnedSegment = Cast<AMovingSegment>(SpawnedActor);
		if (SpawnedSegment) {
			SpawnedSegment->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedSegment->Init(this);
			Segments_.Add(SpawnedSegment);

			ABarricade* SpawnedBarricade = SpawnedSegment->SpawnBarricade(BarricadeClass);
			if (SpawnedBarricade) {
				Barricades_.Add(SpawnedBarricade);
			}
		}
	}
}

bool AMovingLine::CheckFirstSegmentForDelete() {
	if (Segments_.IsValidIndex(0)) {
		FVector NextLineSpawnLocationInWorld = Segments_[0]->GetNextSegmentSpawnLocation();
		
		float XEndPosiotion = Segments_[0]->GetSegmentSize() * -MemorySize_;
		return NextLineSpawnLocationInWorld.X < (XEndPosiotion - 50.0f);
	}

	return false;
}

bool AMovingLine::CheckSegmentByDistance(int SegmentIndex, float Distance) const{
	if (Segments_.IsValidIndex(SegmentIndex)) {
		FVector NextLineSpawnLocationInWorld = Segments_[SegmentIndex]->GetNextSegmentSpawnLocation();

		return NextLineSpawnLocationInWorld.X < Distance;
	}

	return false;
}

AMovingLevel* AMovingLine::GetLevel(){
	return Level_;
}

void AMovingLine::SetMemorySize(int Size){
	MemorySize_ = Size;
}

void AMovingLine::DeleteFirstSegment(){
	if (Segments_.IsValidIndex(0)) {
		Segments_[0]->Cleanup();
		Segments_[0]->Destroy();
		Segments_.RemoveAt(0);
	}
}

TArray<AMovingSegment*>& AMovingLine::GetSegments() {
	return Segments_;
}

FVector AMovingLine::GetCharacterStandLocation(){
	return GetActorLocation() + CharacterStandLocation_;
}

AMovingLine* AMovingLine::GetLeftLine(){
	return LeftLine_;
}

AMovingLine* AMovingLine::GetRightLine(){
	return RightLine_;
}

void AMovingLine::SetLeftLine(AMovingLine* Line){
	LeftLine_ = Line;
}

void AMovingLine::SetRightLine(AMovingLine* Line){
	RightLine_ = Line;
}
