#pragma once
#include "Day6.h"

using TimeAndDistance = std::pair<long long, long long>;

std::vector<TimeAndDistance> getTimeAndDistances(std::ifstream& file)
{
    std::vector<TimeAndDistance> races;
    std::string str;
    std::getline(file, str); //get first line (times)
    std::stringstream ss(str);
    std::string word;
    ss >> word; //skip "Time:"

    //get times
    while (ss >> word)
    {
        auto time = std::stoi(word);
        races.push_back({ time, 0 });
    }

    std::getline(file, str); //get second line (distances)
    ss.clear();
    ss.str(str);
    ss >> word; //skip "Distance:"

    int index = 0;
    while (ss >> word)     //get distances
    {
        auto distance = std::stoi(word);
        races[index].second = distance;
        index++;
    }
    return races;
}

std::vector<int> bruteForceMath(std::vector<TimeAndDistance>& races)
{
    std::vector<int> allWinsTotals;
    for (auto pair : races)
    {
        int winningTimes = 0;
        for (int i = 0; i <= pair.first; i++) //brute force solutions to given math problem
        {
            long long x = pair.first;
            long long s = x - i;
            long long t = x - s;
            //Quadratic formula? never heard of it :) 
            if ((x * x) - (s * x) - (t * x) + (s * t) > pair.second)
            {
                winningTimes++;
            }
        }
        allWinsTotals.push_back(winningTimes);
    }
    return allWinsTotals;
}

int multiplyWins(std::vector<int>& allWinsTotals)
{
    int allWinsMultiplied = 1;
    for (int wins : allWinsTotals)
    {
        allWinsMultiplied *= wins;
    }
    return allWinsMultiplied;
}

int Day6::solvePart1()
{
    std::ifstream file("2023/Day6.txt");
    std::vector<TimeAndDistance> races = getTimeAndDistances(file); //parse the races from the file
    std::vector<int> allWinsTotals = bruteForceMath(races); //calculate the win totals
    return multiplyWins(allWinsTotals); //return the win total bonus maths
}

std::pair<long long, long long> convertMultipleRacesToOne(std::vector<TimeAndDistance>& races)
{
    std::pair<long long, long long> oneRace;
    std::string time = "";
    std::string distance = "";
    //convert to one race, based on smashing numbers together because obviously the elf's plans were unclear
    for (auto pair : races) //string concat to merge to one number
    { 
        time += std::to_string(pair.first); //34 35 36 -> 343536
        distance += std::to_string(pair.second);
    }

    oneRace.first = std::stoll(time);
    oneRace.second = std::stoll(distance);
    return oneRace;
}

int Day6::solvePart2()
{
    std::vector<TimeAndDistance> races;
    std::ifstream file("2023/Day6.txt");

    races = getTimeAndDistances(file); //parse the input file
    TimeAndDistance oneRace = convertMultipleRacesToOne(races); //smash together to form one race
    std::vector<TimeAndDistance> oneRaceV;
    oneRaceV.push_back(oneRace); //put in a vector just to fit existing method
    std::vector<int> allWinsTotals = bruteForceMath(oneRaceV); //calculate the win totals via brute force
    return allWinsTotals[0]; //should check out of index here but its Christmas lighten up.
}

int Day6::solve()
{
    std::cout << "Solving Day 6" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};



