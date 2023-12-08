#pragma once
#include "Day3.h"

//////////////////////
//
//Part 1
//////////////////////
int Day3::solvePart1()
{
    std::ifstream file("2023/Day3.txt");
    std::string previousStr;
    std::string middleStr;
    std::string nextStr;
    int totalGears = 0;
    while (std::getline(file, nextStr))
    {
        totalGears += checkLine(previousStr, middleStr, nextStr); //three strings, with the middle being the focus
        previousStr = middleStr;
        middleStr = nextStr;
    }
    totalGears += checkLine(previousStr, middleStr, nextStr); //need to check the very last line against the previous.
    return totalGears;
}

int Day3::checkLine(std::string& previousString, std::string& middleString, std::string& nextString)
{
    int gears = 0;
    for (int i = 0; i < middleString.length(); i++)
    {
        if (!std::isdigit(middleString[i])) continue; //looking for digits
        int potential = std::stoi(middleString.substr(i, middleString.length())); //will ignore trailing characters  234... to 234
        int numDigits = std::to_string(potential).length();

        //check this line
        int startIndex = i - 1 > 0 ? i - 1 : i + numDigits; //when start index is out of bounds (at start of line), just ignore (double up with endIndex)
        int endIndex = i + numDigits < middleString.length() ? i + numDigits : startIndex; //similar to above, but when endIndex is out of range 
        if (middleString[startIndex] != '.' || middleString[endIndex] != '.')
        {
            gears += potential;
            i += numDigits; //advance the iterator past the number we just checked
            continue; //all done! on the the next one
        }
        
        //if no symbol in this line, check the previous and next lines
        if (checkAdjacentLine(previousString, i, numDigits)) { gears += potential; }
        else if (checkAdjacentLine(nextString, i, numDigits)) { gears += potential; } //else to prevent double count (though there is no number touching two symbols, it seems).

        i += numDigits; //advance the iterator past the number we just checked
    }
    return gears;
}

bool Day3::checkAdjacentLine(std::string& str, int index, int numDigits)
{
    if (str.empty()) return false;
    int startIndex = index - 1 > 0 ? index - 1 : 0; //use 0 if startIndex is out of bounds (at start of line)
    int length = index + numDigits + 2 < str.length() ? numDigits + 2 : numDigits + 1; //+2 length for the 2 extra diagonal indices, or just +1 if end of line
    for (char symbol : str.substr(startIndex, length))
    {
        if (symbol != '.' && !std::isdigit(symbol))
        {
            return true;
        }
    }
    return false; //no symbol found
}

//////////////////////
//
//Part 2
//////////////////////
int Day3::solvePart2()
{
    std::ifstream file("2023/Day3.txt");
    std::string previousStr;
    std::string middleStr;
    std::string nextStr;
    int totalGears = 0;
    while (std::getline(file, nextStr))
    {
        totalGears += checkLinePart2(previousStr, middleStr, nextStr);  //three strings, with the middle being the focus
        previousStr = middleStr;
        middleStr = nextStr;
    }
    totalGears += checkLinePart2(previousStr, middleStr, nextStr); //need to check the very last line against the previous.
    return totalGears;
}

int Day3::checkLinePart2(std::string& previousString, std::string& middleString, std::string& nextString)
{
    int gears = 0;
    for (int i = 0; i < middleString.length(); i++)
    {
        if (middleString[i] != '*') continue; //looking for *

        std::vector<int> touchingGears;
        if (i > 0 && std::isdigit(middleString[i - 1]))
        {
            //found a number preceeding *
            int j = i;
            std::string prefixString;
            while (j >= 0 && std::isdigit(middleString[--j]))
            {
                prefixString += middleString[j]; //building number backwards
            }
            std::reverse(prefixString.begin(), prefixString.end());
            touchingGears.push_back(std::stoi(prefixString));
        }
        if (std::isdigit(middleString[i + 1]))
        {
            //found an adjacent number following *
            touchingGears.push_back(std::stoi(middleString.substr(i+1, middleString.length()))); //string is foreward
        }

        //symbol in this line, check the previous and next lines for gears. Technically you could get >2 matches, but data is not set up that way.
        std::vector<int> matches;
        matches = checkAdjacentLinePart2(previousString, i);
        touchingGears.insert(std::end(touchingGears), std::begin(matches), std::end(matches));
        matches = checkAdjacentLinePart2(nextString, i);
        touchingGears.insert(std::end(touchingGears), std::begin(matches), std::end(matches));
        if (touchingGears.size() == 2)
        {
            gears += touchingGears[0] * touchingGears[1];
        }
    }
    return gears;
}

//can get multiple matches per line
std::vector<int> Day3::checkAdjacentLinePart2(std::string& str, int index)
{
    std::vector<int> returnMatches;
    if (str.empty()) return returnMatches;
    for (int i = 0; i < str.length(); i++)
    {
        if (!std::isdigit(str[i])) continue; //looking for digits
        int potential = std::stoi(str.substr(i, str.length())); //will ignore trailing characters  234... to 234
        int numDigits = std::to_string(potential).length();
        int startIndex = i - 1; //-1 for diagonal relation
        int finishIndex = i + numDigits;
        if (index >= startIndex && index <= finishIndex) //check if middle line index is within this potential gear numbers index range
        {
            returnMatches.push_back(potential);
        }
        i += numDigits; //move iterator past already parsed number
    }
    return returnMatches; 
}

int Day3::solve()
{
    std::cout << "Solving Day 3" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}


