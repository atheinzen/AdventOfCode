#pragma once
#include "Day2.h"

int Day2::solvePart1()
{
    std::ifstream file("2023/Day2.txt");
    std::string str;
    std::string word;

    int gameIdSum = 0;
    while (std::getline(file, str))
    {
        std::stringstream ss(str);
        ss >> word; //skip "Game" 
        ss >> word; // "1:"
        int currentGameId = std::stoi(word);

        //
        //if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes
        //

        int maxRed = 12;
        int maxGreen = 13;
        int maxBlue = 14;
        
        bool fail = false;
        while (ss >> word)
        {
            int amount = std::stoi(word);
            ss >> word;
            std::string color = word;
            if (color.starts_with("red") && amount > maxRed) { fail = true; break; } //even one violation fails whole game
            if (color.starts_with("green") && amount > maxGreen) { fail = true; break; }
            if (color.starts_with("blue") && amount > maxBlue) { fail = true; break; }
        }
        if(!fail) gameIdSum += currentGameId;
    }
    return gameIdSum;
}

int Day2::solvePart2()
{
    std::ifstream file("2023/Day2.txt");
    std::string str;
    std::string word;

    int gameIdPowerSum = 0;
    while (std::getline(file, str))
    {
        std::stringstream ss(str);
        ss >> word; //skip "Game" 
        ss >> word; //skip "1:"

        //
        //fewest number of cubes of each color
        //

        int maxRed = 0;
        int maxGreen = 0;
        int maxBlue = 0;

        while (ss >> word)
        {
            int amount = std::stoi(word);
            ss >> word;
            std::string color = word;
            if (color.starts_with("red") && amount > maxRed) { maxRed = amount; }
            if (color.starts_with("green") && amount > maxGreen) { maxGreen = amount; }
            if (color.starts_with("blue") && amount > maxBlue) { maxBlue = amount; }
        }
        gameIdPowerSum += (maxRed * maxGreen * maxBlue);
    }
    return gameIdPowerSum;
}

int Day2::solve()
{
    std::cout << "Solving Day 2" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}



