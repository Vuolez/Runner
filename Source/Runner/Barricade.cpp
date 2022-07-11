#include "Barricade.h"

ABarricade::ABarricade(){
}

void ABarricade::Init(AMovingLevel* Level){
	Level_ = Level;
}

AMovingLevel* ABarricade::GetLevel(){
	return Level_;
}

void ABarricade::BeginPlay(){
	Super::BeginPlay();
}