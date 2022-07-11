#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include <random>
#include <vector>
#include <map>

#include "StaticLibrary.generated.h"


UCLASS()
class RUNNER_API UStaticLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static std::vector<std::vector<int>> EmptyMap_;
	static std::map<int, std::vector<std::vector<std::vector<char>>>> Maps_;
	
	//static std::random_device Rd_;
	
	//static int GetRandomInt(int Min, int Max);
};
