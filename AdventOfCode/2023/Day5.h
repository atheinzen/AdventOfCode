#pragma once
#include "../Day.h"

#include <fstream>
#include <string>
#include <list>
#include <iostream>
#include <vector>
#include <sstream>
#include <climits>
#include <numeric>
#include <set>
#include <map>
#include <algorithm>

class Day5 : public Day
{
private:
	long long solvePart1();
	long long solvePart2();
public:
	int solve();
};