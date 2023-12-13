#include "Day12.h"
#include <bitset>

using InputPair = std::pair<std::string, std::vector<int>>;

std::vector<int> splitString(std::string& springs)
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
    splitString.push_back(std::stoi(buildString));
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
        pair.first = word;
        ss >> word;
        pair.second = splitString(word);
        allLines.push_back(pair);
    }
    return allLines;
}

std::string& trim(std::string& s, char c, bool reverse = false)
{
    return reverse
        ? s.erase(s.find_last_not_of(c) + 1)
        : s.erase(0, s.find_first_not_of(c));
}



std::string stripExcessPeriods(std::string& springs)
{
    trim(springs, '.', true);
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

std::string buildMatchString(std::vector<int> brokens)
{
    std::string matchString;
    for (auto num : brokens)
    {
        matchString += std::string(num, '#') + '.';
    }
    trim(matchString, '.', true); //trim trailing period
    return matchString;
}


std::vector<std::string> generateAllPossibleStrings(std::string& input)
{
    int questionMarks = 0;
    for (auto c : input)
    {
        if (c == '?') questionMarks++;
    }

    std::vector<std::string> allStrings;
    for (int i = 0; i < std::pow(2,questionMarks); i++)
    {
        std::bitset<32> bs(i);
        std::string bitString(bs.to_string());
        std::reverse(bitString.begin(), bitString.end());
        std::string buildString;
        int countQs = 0;
        for (int j = 0; j < input.size(); j++)
        {
            if (input[j] == '?')
            {
                buildString += (bitString[countQs] == '0' ? '.' : '#');
                countQs++;
            }
            else
            {
                buildString += input[j];
            }
        }
        allStrings.push_back(stripExcessPeriods(buildString));
    }
    return allStrings;
}


long long checkPossibleMatches(std::string& springs, std::string& find)
{
    auto allStrings = generateAllPossibleStrings(springs);
    int matches = 0;
    for (auto possible : allStrings)
    {
        if (possible == find)
        {
            matches++;
        }
    }
    return matches;
}

InputPair folder(std::string& springs, std::vector<int>& brokens, int folds)
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

int getMatches(std::string& springs, std::vector<int> brokens)
{
    std::string dedupedSprings = stripExcessPeriods(springs);
    std::string matchString = buildMatchString(brokens);
    return checkPossibleMatches(dedupedSprings, matchString);
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
    for (auto pair : fiveFoldInputs)
    {
        runningCount += getMatches(pair.first, pair.second);
    }
    return runningCount;
} 

long long Day12::solvePart1()
{
    std::ifstream file("2023/Day12.txt");
    std::vector<InputPair> inputPairs = parseInput(file);
    long long runningCount = 0;
    for (auto pair : inputPairs)
    {
        runningCount += getMatches(pair.first, pair.second);
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

//int getMatchesRecursive(std::string& springs, std::vector<int> brokens)
//{
//    //  if (brokens.size() == 0) return nullptr;
//
//    auto max = *max_element(std::begin(brokens), std::end(brokens));
//    int preceeding = 0;
//    int following = 0;
//    bool postMax = false;
//    std::vector<int> preceedingInts;
//    std::vector<int> followingInts;
//    for (int num : brokens)
//    {
//        if (num == max && !postMax)
//        {
//            postMax = true;
//            continue;
//        }
//        if (!postMax)
//        {
//            preceeding += num + 1;
//            preceedingInts.push_back(num);
//        }
//        else
//        {
//            following += num + 1;
//            followingInts.push_back(num);
//        }
//    }
//    auto checkString = springs.substr(preceeding, (springs.length() - following) - preceeding);
//    auto indices = checkPossibleMatches(checkString, std::string(max, '#'));
//
//    long long preceedingCount = 0;
//    long long followingCount = 0;
//    for (auto index : indices)
//    {
//        std::pair<int, int> outOfBounds{ index.first + preceeding, index.second + preceeding };
//        std::string preceedingString = springs.substr(0, outOfBounds.first);
//
//        if (preceedingInts.size() > 0 && preceedingString.size() > preceeding)
//        {
//            preceedingCount += getMatchesRecursive(preceedingString, preceedingInts);
//        }
//        else if (preceedingInts.size() > 0)
//        {
//            preceedingCount += 1;
//        }
//        std::string followingString = springs.substr(outOfBounds.second, springs.length() - outOfBounds.second);
//        if (followingInts.size() > 0 && followingString.size() > following)
//        {
//            followingCount += getMatchesRecursive(followingString, followingInts);
//        }
//        else if (followingInts.size() > 0)
//        {
//            followingCount += 1;
//        }
//    }
//    return preceedingCount + followingCount;
//}

//std::vector<std::pair<int, int>> checkPossibleMatches(std::string& springs, std::string_view find)
//{
//    std::vector<std::pair<int, int>> startAndEndIndexes;
//    for (int i = 0; i < springs.size(); i++)
//    {
//        std::string replaced = replaceString(springs, "?", "#");
//        if (replaced.substr(i, find.length()) == find) startAndEndIndexes.push_back({ i, i + find.length() });
//    }
//    return startAndEndIndexes;
//}

//buildSpringSplit += springSplit;
//for (auto findSplit : findSplits)
//{
//    buildFindSplit += findSplit;
//    for (int i = 0; i < buildSpringSplit.size(); i++)
//    {
//        std::string springSub = buildSpringSplit.substr(i, findSplit.length());
//        bool isMatch = true;
//        for (int j = 0; j < springSub.size(); j++)
//        {
//            if (!charMatch(buildFindSplit[j], springSub[j])) isMatch = false;
//        }
//        matches += isMatch ? 1 : 0;
//        buildFindSplit += ".";
//    }
//}
//buildSpringSplit += '.';