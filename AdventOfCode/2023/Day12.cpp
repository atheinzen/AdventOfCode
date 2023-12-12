#include "Day12.h"

using InputPair = std::pair<std::string, std::vector<int>>;

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
        pair.first = word;
        ss >> word;
        for (auto ch : word)
        {
            if (std::isdigit(ch)) pair.second.push_back(ch - '0');
        }
        allLines.push_back(pair);
    }
    return allLines;
}

std::string replaceString(std::string subject, const std::string& search,
    const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

std::vector<std::pair<int, int>> checkPossibleMatches(std::string& springs, std::string_view find)
{
    std::vector<std::pair<int, int>> startAndEndIndexes;
    for (int i = 0; i < springs.size(); i++)
    {
        std::string replaced = replaceString(springs, "?", "#");
        if (replaced.substr(i, find.length()) == find) startAndEndIndexes.push_back( { i, i+find.length() });
    }
    return startAndEndIndexes;
}

std::vector<std::pair<int, int>> getMatchesRecursive(std::string& springs, std::vector<int> brokens)
{
  //  if (brokens.size() == 0) return nullptr;

    auto max = *max_element(std::begin(brokens), std::end(brokens));
    int preceeding = 0;
    int following = 0;
    bool postMax = false;
    std::vector<int> preceedingInts;
    std::vector<int> followingInts;
    for (int num : brokens)
    {
        if (num == max) 
        {
            postMax = true; 
            continue;
        }
        if (!postMax)
        {
            preceeding += num + 1;
            preceedingInts.push_back(num);
        }
        else
        {
            following += num + 1;
            followingInts.push_back(num);
        }
    }
    auto checkString = springs.substr(preceeding, (springs.length() - following)-preceeding);
    auto indices = checkPossibleMatches(checkString, std::string(max, '#'));

    long long count = 0;
    std::vector<std::pair<int, int>> linePairs;
    for (auto index : indices)
    {
        linePairs.push_back(index);
        //count++;
        if (preceedingInts.size() > 0)
        {
            std::string preceedingString = springs.substr(0, (index.first + preceeding));
            auto pairs = getMatchesRecursive(preceedingString, preceedingInts);
            linePairs.insert(linePairs.end(), pairs.begin(), pairs.end());
        }
        if (followingInts.size() > 0)
        {
            std::string followingString = springs.substr(preceeding + index.second, springs.length() - (preceeding + index.second));
            auto pairs = getMatchesRecursive(followingString, followingInts);
            linePairs.insert(linePairs.end(), pairs.begin(), pairs.end());
        }
    }
    return linePairs;
}

long long Day12::solvePart1()
{
    std::ifstream file("2023/Day12.txt");
    std::vector<InputPair> inputPairs = parseInput(file);
    long long runningCount = 0;
    for (auto pair : inputPairs)
    {
        getMatchesRecursive(pair.first, pair.second);
    }
    return runningCount;
} 

long long Day12::solvePart2()
{
    return 0;
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