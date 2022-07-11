#include "MovableActor.h"
#include "Kismet/GameplayStatics.h"

AMovableActor::AMovableActor(){
	PrimaryActorTick.bCanEverTick = true;
}

void AMovableActor::BeginPlay(){
	Super::BeginPlay();
}

void AMovableActor::Tick(float DeltaTime){
	
	if (CheckFirstSegmentForDelete()) {
		DeleteFirstSegment();
		SpawnNextSegment();
	}

	Super::Tick(DeltaTime);
}

void AMovableActor::Init(AMovingLevel* MovingLevel){
	MovingLevel_ = MovingLevel; 

	AttachToComponent(MovingLevel_->MovingComp_, FAttachmentTransformRules::KeepWorldTransform);

	for (size_t i = 0; i < SegmentCount_; ++i) {
		SpawnNextSegment();
	}
}

void AMovableActor::SpawnNextSegment() {
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	FVector Location = Segments_.IsValidIndex(0) ? Segments_.Last()->GetNextSegmentSpawnLocation() : GetActorLocation();

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SegmentClass_, Location, Rotation, SpawnInfo);
	if (SpawnedActor) {
		AMovingSegment* SpawnedSegment = Cast<AMovingSegment>(SpawnedActor);
		if (SpawnedSegment) {
			SpawnedSegment->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Segments_.Add(SpawnedSegment);
		}
	}
}

bool AMovableActor::CheckFirstSegmentForDelete() {
	if (Segments_.IsValidIndex(0)) {
		FVector NextLineSpawnLocationInWorld = Segments_[0]->GetNextSegmentSpawnLocation();

		float XEndPosiotion = Segments_[0]->GetSegmentSize() * -MovingLevel_->GetMemorySize();
		return NextLineSpawnLocationInWorld.X < (XEndPosiotion - 50.0f);
	}

	return false;
}

void AMovableActor::DeleteFirstSegment() {
	if (Segments_.IsValidIndex(0)) {
		Segments_[0]->Destroy();
		Segments_.RemoveAt(0);
	}
}

bool AMovableActor::CheckSegmentByDistance(int SegmentIndex, float Distance) const {
	if (Segments_.IsValidIndex(SegmentIndex)) {
		FVector NextLineSpawnLocationInWorld = Segments_[SegmentIndex]->GetNextSegmentSpawnLocation();

		return NextLineSpawnLocationInWorld.X < Distance;
	}

	return false;
}
