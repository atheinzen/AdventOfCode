#pragma once
#include "../Day.h"

#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<numeric>

class Day1 : public Day
{
private:
	int solvePart1();
	int checkWord(std::string_view potential);
	int solvePart2();

public:
	int solve();
};