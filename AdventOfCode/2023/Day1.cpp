#include "Day1.h"

int solvePart1()
{
    std::ifstream file("2023/Day1.txt");
    std::string str;

    //parse and calculate input
    int calibSum = 0;
    while (std::getline(file, str))
    {
        std::string firstDigit;
        std::string secondDigit;
        for (int i = 0; i < str.length(); i++) 
        {
            if (std::isdigit(str[i]))
            {
                if (firstDigit.empty())
                {
                    firstDigit = str[i];
                }
                secondDigit = str[i]; //can be same as first, if no other number.
            }
        }
        calibSum += std::stoi(firstDigit + secondDigit); //string concat, then conversion
    }
    return calibSum;
}

int checkWord(std::string_view potential)
{
    if (potential.find("one") != std::string::npos) return 1;
    if (potential.find("two") != std::string::npos) return 2;
    if (potential.find("three") != std::string::npos) return 3;
    if (potential.find("four") != std::string::npos) return 4;
    if (potential.find("five") != std::string::npos) return 5;
    if (potential.find("six") != std::string::npos) return 6;
    if (potential.find("seven") != std::string::npos) return 7;
    if (potential.find("eight") != std::string::npos) return 8;
    if (potential.find("nine") != std::string::npos) return 9;
    return 0;
}

int solvePart2()
{
    std::ifstream file("2023/Day1.txt");
    std::string str;

    //parse input
    int  calibValues = 0;
    while (std::getline(file, str))
    {
        //find the first digit
        std::string firstDigit;
        std::string potentialWord;
        for (int i = 0; i < str.length(); i++)
        {
            if (std::isdigit(str[i]))
            {
                firstDigit = str[i];
                break;
            }
            potentialWord += str[i];
            auto wordValue = checkWord(potentialWord);
            if (wordValue)
            {
                firstDigit = std::to_string(wordValue);
                break;
            }
        }

        //find the second digit
        potentialWord = "";
        std::string secondDigit;
        for (int i = str.length() - 1; i >= 0; i--) //reverse order
        {
            if (std::isdigit(str[i]))
            {
                secondDigit = str[i];
                break;
            }
            potentialWord = str[i] + potentialWord; //reverse: x ix six
            auto wordValue = checkWord(potentialWord);
            if (wordValue)
            {
                secondDigit = std::to_string(wordValue);
                break;
            }
        }
        calibValues += std::stoi(firstDigit + secondDigit); //string concat, then conversion
    }

    return calibValues;
}

int Day1::solve() 
{
    std::cout << "Solving Day 1" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};