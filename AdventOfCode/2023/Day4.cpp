#pragma once
#include "Day4.h"

int getWins(const std::string& str)
{
    std::string word;
    std::stringstream ss(str);
    ss >> word; //skip Card
    ss >> word; //skip 1:
    int wins = 0;
    while (ss >> word)
    {
        if (word == "|") break; //only check numbers before deliminator
        if (word.length() == 1) //need special attention for 1 digit numbers
        {
            word = " " + word + " "; //add blank spaces to prevent 7 from matchin 71 or 17.
        }
        //a bit hacky, add white space at end for sinle digit number matching (otherwise " 7 " wont match " 7" at end of line
        std::string checkString = str.substr(str.find("|"), str.length() - str.find("|")) + " "; //from delim to end of string.
        if (checkString.find(word) != std::string::npos)
        {
            wins++;
        }
    }
    return wins;
}

int Day4::solvePart1()
{
    std::ifstream file("2023/Day4.txt");
    std::string str;
    std::string word;

    int totalWins = 0;
    while (std::getline(file, str))
    {
        int wins = getWins(str);
        totalWins += std::pow(2, wins - 1); //calculate bonus, 2^-1 ignored.
    }
    return totalWins;
}


int Day4::solvePart2()
{
    std::ifstream file("2023/Day4.txt");
    std::string str;
    std::string word;
    std::deque<int> bonusQueue;
    int totalCards = 0;
    while (std::getline(file, str))
    {
        int wins = getWins(str);

        int multiplier = 1; //multiplier of wins if there are copies. 
        if (bonusQueue.size() > 0)
        {
            multiplier += bonusQueue[0]; //if we have copies, add them here.
            bonusQueue.pop_front(); 
        }
        totalCards += (wins * multiplier) + 1; //plus one for the original copy of the card (which is independant of wins or bonus cards)

        while (bonusQueue.size() < wins) { bonusQueue.push_back(0); }
        for (int i = 0; i < wins; i++) //add copies to future cards
        {
            bonusQueue[i] += 1 * multiplier; //add a card for each copy of this card
        }
    }
    return totalCards;
}

int Day4::solve()
{
    std::cout << "Solving Day 4" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}

