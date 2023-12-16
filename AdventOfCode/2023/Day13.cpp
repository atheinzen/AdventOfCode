#include "Day13.h"

//
// A few days behind so its not optimized 
//

std::vector<std::vector<std::string>> parseInput(std::ifstream& file)
{
    std::string str;
    std::string word;
    std::vector<std::string> grid;
    std::vector <std::vector<std::string>> allGrids;
    while (std::getline(file, str))
    {
        if (str.empty())
        {
            allGrids.push_back(grid);
            grid.clear();
            continue;
        }
        grid.push_back(str);
    }
    allGrids.push_back(grid); //grab the last one
    return allGrids;
}

//create a mirror image of the grids so that the same methods can be used for both
std::vector<std::vector<std::string>> flipGrids(std::vector<std::vector<std::string>>& grids)
{
    std::vector<std::vector<std::string>> flippedGrids;
    for (std::vector<std::string> grid : grids)
    {
        std::vector<std::string> flippedGrid (grid[0].length(), "");
        for(int i=0; i< grid[0].length(); i++)
        {
            for (int j = 0; j < grid.size(); j++)
            {
                flippedGrid[i] += grid[j][i];
            }
        }
        flippedGrids.push_back(flippedGrid);
    }
    return flippedGrids;
}

//recursive check of outward rows
bool mirrorRowsMatch(std::vector<std::string>& grid, int firstRowIndex, int secondRowIndex)
{
    if (firstRowIndex < 0 || secondRowIndex >= grid.size()) return true;
    if (grid[firstRowIndex] == grid[secondRowIndex]) return mirrorRowsMatch(grid, firstRowIndex - 1, secondRowIndex + 1);
    return false;
}

//find the adjacent mirror rows
int findMatchingRows(std::vector<std::string>& grid)
{
    for (int i = 0; i < grid.size() - 1; i++)
    {
        if (grid[i] == grid[i + 1] && mirrorRowsMatch(grid, i, i + 1))
        {
            return i + 1;
        }
    }
    return 0;
}

int countDifferences(std::string& firstString, std::string& secondString)
{
    int diffs = 0;
    for (int i = 0; i < firstString.length(); i++)
    {
        if (firstString[i] != secondString[i])
        {
            diffs++;
        }
    }
    return diffs;
}

//recursive check of outward rows, if only one differing character, a potential match for part 2
int countMirrorRowDifferences(std::vector<std::string>& grid, int firstRowIndex, int secondRowIndex)
{
    if (firstRowIndex < 0 || secondRowIndex >= grid.size()) return 0;
    return countDifferences(grid[firstRowIndex], grid[secondRowIndex]) + countMirrorRowDifferences(grid, firstRowIndex - 1, secondRowIndex + 1);
}

int findMatchingPart2(std::vector<std::string>& grid)
{
    for (int i = 0; i < grid.size() - 1; i++) //for each row
    {
        //if there is only one difference of two adjacent rows, check outward rows (which must be perfect mirrors as we can only change one character)
        bool firstCondition = countDifferences(grid[i], grid[i + 1]) == 1 && mirrorRowsMatch(grid, i - 1, i + 2);
        //if this row matches, check if outward rows have one total difference.
        bool secondCondition = grid[i] == grid[i + 1] && countMirrorRowDifferences(grid, i, i + 1) == 1;
        if (firstCondition || secondCondition)
        {
            return i+1;
        }
    }
    return 0;
}

long long Day13::solvePart1()
{
    std::ifstream file("2023/Day13.txt");
    std::vector<std::vector<std::string>> inputGrids = parseInput(file);
    std::vector<std::vector<std::string>> flippedGrids = flipGrids(inputGrids);
    long long runningCount = 0;
    int puzzle = 1;
    for (int i = 0; i < inputGrids.size(); i++)
    {
        int count = 0;
        count += findMatchingRows(inputGrids[i]) * 100;
        if (count == 0) count += findMatchingRows(flippedGrids[i]); //try columns if not in rows
        std::cout << puzzle << " " << count << std::endl;
        runningCount += count;
        puzzle++;
    }
    return runningCount;
}

long long Day13::solvePart2()
{
    std::ifstream file("2023/Day13.txt");
    std::vector<std::vector<std::string>> inputGrids = parseInput(file);
    std::vector<std::vector<std::string>> flippedGrids = flipGrids(inputGrids);
    long long runningCount = 0;
    int puzzle = 1;
    for (int i = 0; i < inputGrids.size(); i++)
    {
        int count = 0;
        count += findMatchingPart2(inputGrids[i]) * 100;
        if (count == 0) count += findMatchingPart2(flippedGrids[i]); //try columns if not in rows
        std::cout << puzzle << " " << count << std::endl;
        runningCount += count;
        puzzle++;
    }
    return runningCount;
} 

int Day13::solve()
{
    std::cout << "Solving Day 13" << std::endl;

   auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};