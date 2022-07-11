#include "BarricadeManager.h"
#include "StaticLibrary.h"

void UBarricadeManager::Init(const int LinesCount, const FBarricadeInfo& BarricadeInfo) {
	LinesCount_ = LinesCount;
	BarricadeInfo_ = BarricadeInfo;
}

TArray<TSubclassOf<ABarricade>> UBarricadeManager::GetNexRow(){
	TArray<TSubclassOf<ABarricade>> Row;
	
	if (CurrentMap_.empty() || CurrentRowIndex_ <= -1) {
		CurrentMap_ = GetNextMap();
		if (!CurrentMap_.empty()) {
			CurrentRowIndex_ = CurrentMap_.size() - 1;
		}
	}

	for (int BarricadeSynbol : CurrentMap_[CurrentRowIndex_]) {
		switch (BarricadeSynbol)
		{
		case '8':
			Row.Add(BarricadeInfo_.Up);
			break;
		case '2':
			Row.Add(BarricadeInfo_.Down);
			break;
		case '5':
			Row.Add(BarricadeInfo_.Block);
			break;
		case 'c':
			Row.Add(BarricadeInfo_.Coin);
			break;
		default:
			Row.Add(TSubclassOf<ABarricade>());
			break;
		}
	}

	--CurrentRowIndex_;

	return Row;
}


std::vector<std::vector<char>> UBarricadeManager::GetNextMap() {
	std::vector<std::vector<char>> NextMap;
	if (UStaticLibrary::Maps_.find(LinesCount_) != UStaticLibrary::Maps_.end()) {
		auto MapsCurrentDifficulty = UStaticLibrary::Maps_.at(LinesCount_);
		int NextMapIndex = FMath::RandRange(0, MapsCurrentDifficulty.size() - 1);
		
		NextMap = MapsCurrentDifficulty.at(NextMapIndex);
	}

	return NextMap;
}
