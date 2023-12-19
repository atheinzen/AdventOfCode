#include "Day15.h"

//
// Very sloppy, a few days behind and busy at work!
//

std::vector<std::string> parseDay15Input(std::ifstream& file) //every split in a vector
{
    std::string str;
    std::string word;
    std::vector<std::string> allWords;
    while (std::getline(file, str))
    {
        for (auto c : str)
        {
            if (c == ',')
            {
                allWords.push_back(word);
                word = "";
                continue;
            }
            word += c;
        }
        allWords.push_back(word); //grab last one
    }
    return allWords;
}

int doAlgorithm(std::string& word)
{
    int count = 0;
    for (auto c : word)
    {
        count = ((count + c) * 17) % 256; //specified by todays prompt
    }
    return count;
}

long long Day15::solvePart1()
{
    std::ifstream file("2023/Day15.txt");
    std::vector<std::string> inputWords = parseDay15Input(file);
    int runningTotal = 0;
    for (auto word : inputWords)
    {
        runningTotal += doAlgorithm(word);
    }
    return runningTotal;
}

//letter codes can be variable length (contrary to part 1)
std::string getLetterCode(const std::string& input)
{
    std::string str;
    for (auto c : input)
    {
        if (c != '-' && c != '=')
        {
            str += c;
        }
        else
        {
            return str;
        }
    }
}

//lazy code to find operator
char getOperator(const std::string& input)
{
    for (auto c : input)
    {
        if (c == '-' || c == '=')
        {
            return c;
        }
    }
    return '0';
}

//should be the only digit
int getFocus(const std::string& input)
{
    std::string str;
    for (auto c : input)
    {
        if (std::isdigit(c))
        {
            return (c - '0'); //convert char value to decimal value
        }
    }
    return '0';
}

//TODO split this up a bit
void alterBox(std::map<int, std::map<std::string, int>>& lensMap, std::map<int, std::vector<std::string>>& insertionOrder, std::string& word)
{
    auto letterCode = getLetterCode(word); //"mqh"
    auto operatorChar = getOperator(word); //= or -
    auto focus = getFocus(word); // 1-9
    auto box = doAlgorithm(letterCode); //0-255
    if (lensMap[box].size() == 0 && operatorChar == '=') //if first ever time in box, add it.
    {
        lensMap[box][letterCode] = focus;
        insertionOrder[box].push_back(letterCode); //we know it doesnt already exist
    }
    else //have seen this box before
    {
        if (operatorChar == '=') //add or update
        {
            lensMap[box][letterCode] = focus; //new focus
            if (std::find(insertionOrder[box].begin(), insertionOrder[box].end(), letterCode) == insertionOrder[box].end()) //if first time
            {
                insertionOrder[box].push_back(letterCode); //track the insertion order
            }
        }
        if (operatorChar == '-') //remove if present
        {
            std::map<std::string, int>::iterator iter = lensMap[box].find(letterCode); //find index
            if (iter != lensMap[box].end()) lensMap[box].erase(iter); //erase if exists
            auto it = std::find(insertionOrder[box].begin(), insertionOrder[box].end(), letterCode); //get iterator to spot
            if (it != insertionOrder[box].end()) //if iterator not at end
            {
                insertionOrder[box].erase(it); //erase (it should exist)
            }
        }
    }
}

long long Day15::solvePart2()
{
    std::ifstream file("2023/Day15.txt");
    std::vector<std::string> inputWords = parseDay15Input(file);
    std::map<int, std::vector<std::string>> insertionOrder; //map is ordered by key but we want insertion order instead

    std::map<int, std::map<std::string, int>> lensMap;
    std::map<std::string, int> initSubMap;
    //init lensMap to incrementing keys for box number
    for (int i = 0; i < 256; i++) //problem specifies 256
    {
        lensMap[i] = initSubMap;
    }
    
    //do the moving of lenses
    for (auto word : inputWords)
    {
        alterBox(lensMap, insertionOrder, word);
    }

    //now count "focusing" power
    int runningTotal = 0;
    for (auto box : lensMap)
    {
        if (lensMap[box.first].size() == 0) continue; //empty box
        int total = 0;
        std::vector<std::string> order = insertionOrder[box.first]; //get the order of insertions, stored as letterCodes
        for (int i=0; i< order.size(); i++) //for each order
        {            //1* box code  * index in map * focus power  
            total += (1 + box.first) * (i + 1) * lensMap[box.first][order[i]]; //calculate focus power as per prompt
        }
        runningTotal += total;
    }
    return runningTotal;
}

int Day15::solve()
{
    std::cout << "Solving Day 15" << std::endl;

    auto part1 = Day15::solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = Day15::solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};
