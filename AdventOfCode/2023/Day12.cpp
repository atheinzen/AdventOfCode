#include "Day12.h"

//
//A bit sloppy as i am again busy at work
//

using InputPair = std::pair<std::string, std::vector<int>>;

//i guess weirdly there isnt a better way to split a string on a delim in c++ without using boost or something
std::vector<int> splitString(std::string& springs) // "4,5,12" ->  vector<int> {4,5,13}
{
    std::vector<int> splitString;
    std::string buildString;
    for (auto c : springs)
    {
        if (c == ',')
        {
            splitString.push_back(std::stoi(buildString));
            buildString = "";
            continue;
        }
        buildString += c;
    }
    splitString.push_back(std::stoi(buildString)); //add the last one
    return splitString;
}

std::vector<InputPair> parseInput(std::ifstream& file)
{
    std::string str;
    std::string word;
    std::vector<InputPair> allLines;
    while (std::getline(file, str))
    {
        InputPair pair;
        std::stringstream ss(str);
        ss >> word;
        pair.first = word; //the string
        ss >> word;
        pair.second = splitString(word); //the pattern 
        allLines.push_back(pair);
    }
    return allLines;
}

std::string& trim(std::string& s, char c, bool reverse = false) //trim leading or trailing characters
{
    return reverse
        ? s.erase(s.find_last_not_of(c) + 1)
        : s.erase(0, s.find_first_not_of(c));
}

std::string stripExcessPeriods(std::string& springs) //some optimization possible here
{
    trim(springs, '.', false);
    std::string deduped;
    bool previousWasAPeriod = false;
    for (auto c : springs)
    {
        if (previousWasAPeriod && c == '.') continue;
        deduped += c;
        previousWasAPeriod = c == '.';
    }
    return deduped;
}

std::string buildMatchString(std::vector<int> brokens) //string the represents the matching pattern as defined by the pattern input
{
    std::string matchString;
    for (auto num : brokens)
    {
        matchString += std::string(num, '#') + '.';
    }
    trim(matchString, '.', true); //trim trailing period
    return matchString;
}

InputPair folder(std::string& springs, std::vector<int>& brokens, int folds) //create 4 copies of original input for part 2
{
    std::string newSprings = springs;
    for (int i = 0; i < folds-1; i++)
    {
        newSprings += "?" + springs;
    }
    std::vector<int> newBrokens;
    newBrokens.insert(newBrokens.begin(), brokens.begin(), brokens.end());
    for (int i = 0; i < folds - 1; i++)
    {
        newBrokens.insert(newBrokens.begin(), brokens.begin(), brokens.end());
    }
    return InputPair { newSprings, newBrokens };
}

long long checkAtIndex(std::string currentSprings, const std::string& rawInput,  std::string& matchString, int index, std::map<std::pair<std::string, int>, long long>& checkedAlready)
{
    if (index >= rawInput.length()) //were done
    {
        trim(currentSprings, '.', true); //dont need to match any trailing '.'
        if (currentSprings == matchString)
        {
            return 1;
        }
        return 0;
    }

    currentSprings = stripExcessPeriods(currentSprings); //trim any trailing period if there are two. TODO Optimize
    if (currentSprings == matchString.substr(0, currentSprings.length()) || currentSprings[currentSprings.length()-1] == '.') //a match OR a possible future match
    {
        if (rawInput[index] == '.' || rawInput[index] == '#') //no ?
        {
            currentSprings += rawInput[index];
            return checkAtIndex(currentSprings, rawInput, matchString, index + 1, checkedAlready); //recursive
        }
        if (rawInput[index] == '?')
        {
            long long leftCount = 0;
            long long rightCount = 0;
            auto leftString = currentSprings + ".";
            auto rightString = currentSprings + "#";
            //check '.' tree
            if (checkedAlready.find({ leftString,index }) == checkedAlready.end()) //if weve already checked a string from this index, look it up. 
            {
                leftCount += checkAtIndex(leftString, rawInput, matchString, index + 1, checkedAlready);
                checkedAlready[{leftString, index}] = leftCount; 
            }
            else 
            {
                leftCount = checkedAlready[{leftString, index}];
            }
            //check '$' tree
            if (checkedAlready.find({ rightString,index }) == checkedAlready.end()) //if weve already checked a string from this index, look it up. 
            {
                rightCount += checkAtIndex(rightString, rawInput, matchString, index + 1, checkedAlready);
                checkedAlready[{rightString, index}] = rightCount;
            }
            else
            {
                rightCount = checkedAlready[{rightString, index}];
            }
            return  leftCount+rightCount;
        }
    }
    else 
    {
        return 0;
    }
}

long long getMatches(std::string& springs, std::vector<int> brokens)
{
    std::string dedupedSprings = stripExcessPeriods(springs);
    std::string matchString = buildMatchString(brokens);
    trim(dedupedSprings, '.', true);
    std::string currentString;
    std::map<std::pair<std::string, int>, long long> checkedAlready;
    return checkAtIndex(currentString, dedupedSprings, matchString, 0, checkedAlready);
}

long long Day12::solvePart1()
{
    std::ifstream file("2023/Day12.txt");
    std::vector<InputPair> inputPairs = parseInput(file);
    long long runningCount = 0;
    long long count = 0;
    long long number = 1;
    for (auto pair : inputPairs)
    {
        count = getMatches(pair.first, pair.second);
        std::cout << number << " " << count << std::endl;
        runningCount += count;
        number++;
    }
    return runningCount;
}

long long Day12::solvePart2()
{
    std::ifstream file("2023/Day12.txt");
    std::vector<InputPair> inputPairs = parseInput(file);
    std::vector<InputPair> fiveFoldInputs;
    for (auto pair : inputPairs)
    {
        fiveFoldInputs.push_back(folder(pair.first, pair.second, 5));
    }
    long long runningCount = 0;
    long long count = 0;
    long long number = 1;
    for (auto pair : fiveFoldInputs)
    {
        count = getMatches(pair.first, pair.second);
        std::cout << number << " " << count << std::endl;
        runningCount += count;
        number++;
    }
    return runningCount;
} 

int Day12::solve()
{
    std::cout << "Solving Day 11" << std::endl;

   auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};