#include "Day11.h"

using Coord = std::pair<long long, long long>;

std::vector<std::string> parseInput(std::ifstream& file) //2D Vector
{
    std::string str;
    std::string word;
    std::vector<std::string> allLines;
    while (std::getline(file, str))
    {
        allLines.push_back(str);
    }
    return allLines;
}
std::vector<std::string> getListOfColumns(std::vector<std::string>& lines) //2D Vector with columns as lines
{
    std::vector<std::string> columns (lines.size(), "");
    for (int i = 0; i < lines.size(); i++)
    {
        for (auto line : lines)
        {
            columns[i] += line[i];
        }
    }
    return columns;
}

std::vector<int> getBlankSpaceIndices(std::vector<std::string>& check) //check blank lines
{
    std::vector<int> outIndices;
    for (int i=0; i< check.size(); i++)
    {
        if (check[i].find('#') == std::string::npos)
        {
            outIndices.push_back(i);
        }
    }
    return outIndices;
}

std::vector<Coord> getGalaxyCoords(std::vector<std::string>& lines) //find the '#'
{
    std::vector<Coord> outCoords;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].length(); j++)
        {
            if (lines[i][j] == '#')
            {
                outCoords.push_back({ j , i });
            }
        }
    }
    return outCoords;
}

//where blankLaneWorth represent how much extra a blank is. 
std::vector<Coord> adjustCoordsForBlankSpace(std::vector<Coord>& inCoords, std::vector<int>& blankRows, std::vector<int>& blankColumns, int blankLineWorth)  
{
    std::vector<Coord> outCoords;
    for (auto coord : inCoords)
    {
        long long xAdjustments = 0;
        for (auto column : blankColumns)
        {
            if (coord.first > column) xAdjustments += blankLineWorth;
        }
        long long yAdjustments = 0;
        for (auto row : blankRows)
        {
            if (coord.second > row) yAdjustments += blankLineWorth;
        }
        outCoords.push_back({ coord.first + xAdjustments , coord.second + yAdjustments});
    }
    return outCoords;
}

//method to recursively count the distances between galaxies just once
long long calculateandSumDistances(std::vector<Coord>& coords)
{
    long long sum = 0;
    if (coords.size() > 1)
    {
        Coord thisCoord = coords[0];
        for (auto coord : coords)
        {
            //no diagonal movement, so just coord diffs
            sum += std::abs(thisCoord.first - coord.first);
            sum += std::abs(thisCoord.second - coord.second);
        }
        coords.erase(coords.begin());
        return sum + calculateandSumDistances(coords);
    }
    return 0;
}

int Day11::solvePart1()
{
    std::ifstream file("2023/Day11.txt");
    std::vector<std::string> allLines = parseInput(file);
    std::vector<std::string> allColumns = getListOfColumns(allLines);
    std::vector<int> blankRowIndices = getBlankSpaceIndices(allLines);
    std::vector<int> blankColumnIndices = getBlankSpaceIndices(allColumns);
    std::vector<Coord> galaxyCoords = getGalaxyCoords(allLines);
    std::vector<Coord> galaxyCoordsAdjusted = adjustCoordsForBlankSpace(galaxyCoords, blankRowIndices, blankColumnIndices, 1); //1 extra per blank
    return calculateandSumDistances(galaxyCoordsAdjusted); //recursively calculate and sum distances.
} 

long long Day11::solvePart2()
{
    std::ifstream file("2023/Day11.txt");
    std::vector<std::string> allLines = parseInput(file);
    std::vector<std::string> allColumns = getListOfColumns(allLines);
    std::vector<int> blankRowIndices = getBlankSpaceIndices(allLines);
    std::vector<int> blankColumnIndices = getBlankSpaceIndices(allColumns);
    std::vector<Coord> galaxyCoords = getGalaxyCoords(allLines);
    std::vector<Coord> galaxyCoordsAdjusted = adjustCoordsForBlankSpace(galaxyCoords, blankRowIndices, blankColumnIndices, 999999); //999999 extra per blank
    return calculateandSumDistances(galaxyCoordsAdjusted); //recursively calculate and sum distances
}

int Day11::solve()
{
    std::cout << "Solving Day 11" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};