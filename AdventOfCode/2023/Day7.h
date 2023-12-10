#pragma once
#include "../Day.h"

#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
#include <map>
#include <algorithm>
#include <vector>
#include <functional>

class Day7 : public Day
{
private:
	int solvePart1();
	int solvePart2();
public:
	int solve();
};