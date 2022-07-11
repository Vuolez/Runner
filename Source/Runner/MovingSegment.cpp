#include "MovingSegment.h"
#include "MovingLine.h"

AMovingSegment::AMovingSegment(){
	RootSceneComp_ = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComp_);
}

void AMovingSegment::BeginPlay(){
	Super::BeginPlay();
}

void AMovingSegment::Init(AMovingLine* Line) {
	Line_ = Line;
}

void AMovingSegment::Cleanup(){
	if (Barricade_) {
		Barricade_->Destroy();
	}
}

ABarricade* AMovingSegment::SpawnBarricade(TSubclassOf<ABarricade> BarricadeClass){
	if (BarricadeClass) {
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(BarricadeClass, GetActorLocation() + BarricadeSpawnLocation_, Rotation, SpawnInfo);
		if (SpawnedActor) {
			Barricade_ = Cast<ABarricade>(SpawnedActor);
			Barricade_->Init(Line_->GetLevel());
			if (Barricade_) {
				Barricade_->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}

	return Barricade_;
}

void AMovingSegment::DestroyBarricade() {
	if (Barricade_) {
		Barricade_->Destroy();
	}
}


FVector AMovingSegment::GetNextSegmentSpawnLocation() {
	return GetActorLocation() + NextSegmentSpawnLocation_;
}


float AMovingSegment::GetSegmentSize() {
	return NextSegmentSpawnLocation_.X;
}
