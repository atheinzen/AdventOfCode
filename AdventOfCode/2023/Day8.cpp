#include "Day8.h"

using CoordPair = std::pair<std::string, std::string>;
using MazeMap = std::map<std::string, CoordPair>;

MazeMap parseMazeMap(std::ifstream& file, std::string& directions)
{
    std::string str;
    std::string word;
    std::getline(file, str); //first line, directions;
    directions = str;
    std::getline(file, str); //skip blank line
    MazeMap mMap = MazeMap();
    while (std::getline(file, str)) //coord map
    {
        std::stringstream ss(str);
        ss >> word; //GSM = (TXH, CJK)
        std::string key{ word };
        ss >> word; //skip =
        ss >> word; //(TXH,
        std::string left = word.substr(1, 3);
        ss >> word; // CJK)
        std::string right = word.substr(0, 3);
        mMap[key] = CoordPair{ left, right };
    }
    return mMap;
}

int countRequiredTransitions(std::string& directions, MazeMap mMap, CoordPair currentPair, const std::string& match)
{
    int count = 0;
    while (true)
    {
        for (auto direction : directions)
        {
            count++;
            std::string word = currentPair.first; //set current to left....
            if (direction == 'R')                 //... unless we go right
            {
                word = currentPair.second;
            }
            if (word.ends_with(match)) { return count; } //found it, end_with specific to todays prompt
            currentPair = mMap[word];
        }
    }
    return count; //should not hit
}

int Day8::solvePart1()
{
    std::ifstream file("2023/Day8.txt");
    std::string directions;
    MazeMap mMap = parseMazeMap(file, directions);
    return countRequiredTransitions(directions, mMap, mMap["AAA"], "ZZZ");
}

long long getLCM(long long int1, long long int2)
{
    auto lcm = int1;
    while (1)
    {
        if (lcm % int1 == 0 && lcm % int2 == 0)
        {
            return lcm;
        }
        lcm += int1; //going to be a multiple of at least 1 input. Could optimize further.
    }
}

long long Day8::solvePart2()
{
    std::ifstream file("2023/Day8.txt");
    std::string directions;
    MazeMap mMap = parseMazeMap(file, directions);

    MazeMap currentNodes = MazeMap();
    for (const auto& [key, value] : mMap) //find all that end with 'A'.
    {
        if (key[2] == 'A')
        {
            currentNodes[key] = value;
        }
    }

    std::vector<int> counts;
    for (auto pair : currentNodes) //find the 'cycle' length of each matching starting node.
    {
        counts.push_back(countRequiredTransitions(directions, mMap, pair.second, "Z")); //count until string ends with Z.
    }

    long long lcm = counts[0];
    for (int i = 0; i < counts.size() - 1; i++) //find LCM of first two, then that LCM with next count, and so on.
    {
        lcm = getLCM(lcm, counts[i + 1]);
    }

    return lcm; //the LCM of all six will be where the all end with Z at the same step
}

int Day8::solve()
{
    std::cout << "Solving Day 8" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};