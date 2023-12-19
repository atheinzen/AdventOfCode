#include "Day16.h"

//
// Very sloppy, a few days behind and busy at work!
//
using Coord = std::pair<int, int>;

enum class Direction
{
    NORTH,
    WEST,
    SOUTH,
    EAST
};

std::vector<std::vector<std::string>> parseDay16Input(std::ifstream& file) //a string for each position in the grid.
{
    std::string str;
    std::vector <std::vector<std::string>> gridOfStrings;
    std::vector<std::string> thisLine;
    while (std::getline(file, str))
    {
        for (auto c : str)
        {
            std::string s{ c };
            thisLine.push_back(s); //were gonna make each character a string and then use the string to store the data we need.
        }
        gridOfStrings.push_back(thisLine); 
        thisLine.clear();
    }
    return gridOfStrings;
}

bool outOfBounds(Coord coord, int maxSize)
{
    if (coord.first < 0 || coord.first >= maxSize || coord.second < 0 || coord.second >= maxSize)
    {
        return true;
    }
    return false;
}

bool beenHereTravellingThisWay(std::string str, char dir) //check if we have already visited AND been travelling in this direction, if so, we can stop, its been explored
{
    for (auto c : str)
    {
        if (c == dir)
        {
            return true;
        }
   }
    return false;
}

Coord move(Coord& coord, char Dir) //advance Coord = x,y
{
    switch (Dir) {
        case 'E':
            return { coord.first + 1, coord.second };
        case 'S':
            return { coord.first, coord.second+1 };
        case 'W':
            return { coord.first - 1, coord.second };
        case 'N':
            return { coord.first, coord.second - 1 };
        default:
            return coord;
    }
}

void advanceHeads(std::vector<std::vector<std::string>>& grid, std::vector<std::pair<Coord, char>>& currentHeads, std::vector<std::pair<Coord, char>>& nextHeads)
{
    while (currentHeads.size() != 0) //while we still have heads of the beam we continue
    {
        for (auto head : currentHeads)
        {
            if (outOfBounds(head.first, grid.size())) //kill of heads that have met their end
            {
                //remove head
                continue;
            }
            if (beenHereTravellingThisWay(grid[head.first.second][head.first.first], head.second)) //prevent looping indefinately by checking if weve already been...
            {                                                                                      //.... here and travelling in this direction.
                //remove head
                continue;
            }
            grid[head.first.second][head.first.first] += head.second; //add this direction to end of string i.e. "." -> ".E", can be multiple
            if (grid[head.first.second][head.first.first][0] == '.') //pass through
            {
                nextHeads.push_back({ move(head.first, head.second), head.second });
                continue;
            }
            if (grid[head.first.second][head.first.first][0] == '|') //split possible
            {
                if (head.second == 'N' || head.second == 'S') //dont split, pass through
                {
                    nextHeads.push_back({ move(head.first, head.second), head.second });
                }
                else //split N and S
                {
                    nextHeads.push_back({ move(head.first, 'N'),  'N' });
                    nextHeads.push_back({ move(head.first, 'S'), 'S' });
                }
                continue;
            }
            if (grid[head.first.second][head.first.first][0] == '-') //split possible
            {
                if (head.second == 'E' || head.second == 'W') //dont split, pass through
                {
                    nextHeads.push_back({ move(head.first, head.second), head.second });
                }
                else //split E and W
                {
                    nextHeads.push_back({ move(head.first, 'E'),  'E' });
                    nextHeads.push_back({ move(head.first, 'W'), 'W' });
                }
                continue;
            }
            if (grid[head.first.second][head.first.first][0] == '/') //deflection
            {
                if (head.second == 'S')
                {
                    nextHeads.push_back({ move(head.first, 'W'), 'W' });
                }
                else if (head.second == 'N')
                {
                    nextHeads.push_back({ move(head.first, 'E'), 'E' });
                }
                else if (head.second == 'W')
                {
                    nextHeads.push_back({ move(head.first, 'S'), 'S' });
                }
                else if (head.second == 'E')
                {
                    nextHeads.push_back({ move(head.first, 'N'), 'N' });
                }
                continue;
            }
            if (grid[head.first.second][head.first.first][0] == '\\') //deflection
            {
                if (head.second == 'S')
                {
                    nextHeads.push_back({ move(head.first, 'E'), 'E' });
                }
                else if (head.second == 'N')
                {
                    nextHeads.push_back({ move(head.first, 'W'), 'W' });
                }
                else if (head.second == 'W')
                {
                    nextHeads.push_back({ move(head.first, 'N'), 'N' });
                }
                else if (head.second == 'E')
                {
                    nextHeads.push_back({ move(head.first, 'S'), 'S' });
                }
                continue;
            }
        }
        currentHeads = nextHeads; 
        nextHeads.clear();
    }
}

long long Day16::solvePart1()
{
    std::ifstream file("2023/Day16.txt");
    std::vector<std::vector<std::string>> grid = parseDay16Input(file);
    std::vector<std::pair<Coord, char>> currentHeads;
    std::vector<std::pair<Coord, char>> nextHeads;
    currentHeads.push_back({ { 0, 0 }, 'E' });
    advanceHeads(grid, currentHeads, nextHeads);
    int totalCount = 0;
    for (auto row : grid)
    {
        for (auto column : row)
        {
            if (column.size() > 1) totalCount++; //any visited square counts as one
        }
    }
    return totalCount;
}

long long Day16::solvePart2()
{
    std::ifstream file("2023/Day16.txt");
    std::vector<std::vector<std::string>> grid = parseDay16Input(file);
    std::vector<std::vector<std::string>> workingGrid(grid);
    std::vector<std::pair<Coord, char>> currentHeads;
    std::vector<std::pair<Coord, char>> nextHeads;
    int highCount = 0;
    std::vector<std::pair<Coord, char>> startHeads;
    for (int i = 0; i < grid.size(); i++) //all possible starting directions
    {
        startHeads.push_back({ { 0, i }, 'E' });
        startHeads.push_back({ { i, 0 }, 'S' });
        startHeads.push_back({ { grid.size()-1, i}, 'W'});
        startHeads.push_back({ { i, grid.size() - 1 }, 'N' });
    }

    for (auto startHead : startHeads) //run the process for all possible starting directions.
    {
        currentHeads.push_back(startHead);
        advanceHeads(workingGrid, currentHeads, nextHeads);

        int totalCount = 0;
        for (auto row : workingGrid)
        {
            for (auto column : row)
            {
                if (column.size() > 1) totalCount++;
            }
        }
        if (totalCount > highCount) highCount = totalCount; //only interested in highest possible starting coord and direction
        currentHeads.clear();
        nextHeads.clear();
        workingGrid = grid; //reset the grid for next startHead
    }
    return highCount;
}

int Day16::solve()
{
    std::cout << "Solving Day 16" << std::endl;

    auto part1 = Day16::solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = Day16::solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};
