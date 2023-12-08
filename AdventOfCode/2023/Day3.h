#pragma once
#include "../Day.h"

#include <fstream>
#include <string>
#include <list>
#include <iostream>
#include <vector>

class Day3 : public Day
{
private:
	int checkLine(std::string& previousString, std::string& middleString, std::string& nextString);
	bool checkAdjacentLine(std::string& str, int index, int numDigits);
	int solvePart1();
	int checkLinePart2(std::string& previousString, std::string& middleString, std::string& nextString);
	std::vector<int> checkAdjacentLinePart2(std::string& str, int index);
	int solvePart2();
public:
	int solve();
};