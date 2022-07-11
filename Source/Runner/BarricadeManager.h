#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Barricade.h"

#include <vector>

#include "BarricadeManager.generated.h"

USTRUCT(BlueprintType)
struct FBarricadeInfo {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABarricade> Up;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABarricade> Down;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABarricade> Block;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABarricade> Coin;
};

UCLASS()
class RUNNER_API UBarricadeManager : public UObject
{
	GENERATED_BODY()

public:
	void Init(const int LinesCount, const FBarricadeInfo& BarricadeInfo);
	TArray<TSubclassOf<ABarricade>> GetNexRow();

private:
	int LinesCount_;
	int LevelDifficulty_;
	int CurrentRowIndex_;

	std::vector<std::vector<char>> CurrentMap_;

	size_t CurrentMapIndex_;
	FBarricadeInfo BarricadeInfo_;

	std::vector<std::vector<char>> GetNextMap();
};
