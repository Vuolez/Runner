#include "StaticLibrary.h"

#include <random>

std::vector<std::vector<int>> EmptyMap_;

//5 - Block
//8 - Up Barricade
//2 - Down Barricade
//c - Coin
std::map<int, std::vector<std::vector<std::vector<char>>>> UStaticLibrary::Maps_ =
{ 
	{5, //Map weight
		{
			{
				{'5','0','0','0','5'},
				{'0','5','0','5','0'},
				{'0','8','5','8','0'},
				{'0','2','0','2','0'}
			},
			{
				{'8','2','8','2','8'},
				{'0','0','0','5','0'},
				{'0','5','0','c','0'},
				{'2','0','c','0','2'},
				{'0','c','0','0','0'},
				{'0','5','0','5','0'},
			},
			{
				{'0','5','0','5','0'},
				{'0','c','0','c','0'},
				{'2','8','5','2','8'},
				{'0','2','0','8','0'},
				{'0','0','5','0','0'},
				{'5','0','0','0','5'},
			},
		}
	}
};

//int UStaticLibrary::GetRandomInt(int Min, int Max) {
//	std::mt19937 Rng(Rd_());    // random-number engine used (Mersenne-Twister in this case)
//	std::uniform_int_distribution<int> Uni(Min, Max); // guaranteed unbiased
//
//	return Uni(Rng);
//}