#pragma once
#include "../Day.h"

#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
#include <map>
#include <algorithm>
#include <vector>
#include <numeric>

class Day9 : public Day
{
private:
	int solvePart1();
	long long solvePart2();
public:
	int solve();
};