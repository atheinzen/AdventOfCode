#include "Day9.h"

//recursively generate the next layer, until we get all 0's
int getNextLayer(std::vector<int> numbers, bool nextElem)
{
    std::vector<int> thisLine;
    for (int i=0; i< numbers.size(); i++)
    {
        if (i + 1 >= numbers.size()) break; //no i+1 available, each line will be 1 smaller than previous
        thisLine.push_back(numbers[i+1]-numbers[i]);  //this line is the difference of the numbers in the previous line
    }
    if (std::all_of(thisLine.begin(), thisLine.end(), [](int i) { return i == 0; })) //we are done when all values are 0
    {
        return 0; //start going back up the recursion tree
    }
    if (nextElem) //next element in sequence, or new "first" element;
    {
        return thisLine.back() + getNextLayer(thisLine, nextElem); //looking for next element of the sequence
    }
    else 
    {
        return thisLine.front() - getNextLayer(thisLine, nextElem); //looking for new "first" element of the sequence
    }
    
}

std::vector<std::vector<int>> parseInputs(std::ifstream& file)
{
    std::string str;
    std::string word;
    std::vector<std::vector<int>> allInputs;
    while (std::getline(file, str))
    {
        std::stringstream ss(str);
        std::vector<int> inputs;
        while (ss >> word)
        {
            inputs.push_back(std::stoi(word));
        }
        allInputs.push_back(inputs);
    }
    return allInputs;
}

int Day9::solvePart1()
{
    std::ifstream file("2023/Day9.txt");
    std::vector<std::vector<int>> allInputs = parseInputs(file);
    long long runningTotal = 0;
    for (auto input : allInputs)
    {
        runningTotal += input.back() + getNextLayer(input, true); //get next in sequence
    }
    return runningTotal;
}

long long Day9::solvePart2()
{
    std::ifstream file("2023/Day9.txt");
    std::vector<std::vector<int>> allInputs = parseInputs(file);
    long long runningTotal = 0;
    for (auto input : allInputs)
    {
        runningTotal += input.front() - getNextLayer(input, false); //get previous in sequence
    }
    return runningTotal;
}

int Day9::solve()
{
    std::cout << "Solving Day 9" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};