#include "Day10.h"

using Coord = std::pair<int, int>;

enum class Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

Direction tilesOtherConnection(const char& symbol, const Direction fromDir)
{
    switch (symbol)
    {
    case '|':
    {
        return fromDir == Direction::NORTH ? Direction::SOUTH : Direction::NORTH;
    }
    case '-':
    {
        return fromDir == Direction::EAST ? Direction::WEST : Direction::EAST;
    }
    case 'L':
    {
        return fromDir == Direction::NORTH ? Direction::EAST : Direction::NORTH;
    }
    case 'J':
    {
        return fromDir == Direction::NORTH ? Direction::WEST : Direction::NORTH;
    }
    case '7':
    {
        return fromDir == Direction::SOUTH ? Direction::WEST : Direction::SOUTH;
    }
    case 'F':
    {
        return fromDir == Direction::SOUTH ? Direction::EAST : Direction::SOUTH;
    }
    case '.':
    {
        std::cout << "Should not get this" << std::endl;
        return Direction::SOUTH; //null
    }
    default:
        return Direction::SOUTH; //null
    }
}

bool adjacentTileConnects(const Direction fromDir, const char tile) 
{
    bool connects = false;
    switch (tile) 
    {
        case '|':
        {
            if (fromDir == Direction::NORTH || fromDir == Direction::SOUTH) 
            { 
                connects = true; 
                break; 
            }
        }
        case '-':
        {
            if (fromDir == Direction::EAST || fromDir == Direction::WEST) 
            { 
                connects = true; 
                break; 
            }
        }
        case 'L':
        {
            if (fromDir == Direction::EAST || fromDir == Direction::SOUTH) 
            { 
                connects = true; 
                break; 
            }
        }
        case 'J':
        {
            if (fromDir == Direction::WEST || fromDir == Direction::SOUTH) 
            { 
                connects = true; 
                break; 
            }
        }
        case '7':
        {
            if (fromDir == Direction::WEST || fromDir == Direction::NORTH) 
            { 
                connects = true; 
                break; 
            }
        }
        case 'F':
        {
            if (fromDir == Direction::EAST || fromDir == Direction::NORTH) 
            { 
                connects = true; 
            }
            break;
        }
        case '.':
        {
            std::cout << "Should not get this" << std::endl; 
            break;
        }
        default:
            return false;
    }
    return connects;
}

std::vector<std::vector<char>> parseInput(std::ifstream& file)
{
    std::string str;
    std::string word;
    std::vector<std::vector<char>> allLines;
    while (std::getline(file, str))
    {
        std::vector<char> inputs;
        for (char c : str)
        {
            inputs.push_back(c);
        }
        allLines.push_back(inputs);
    }
    return allLines;
}
Coord getStartPair(const std::vector<std::vector<char>>& allInputs)
{
    for (int y = 0; y < allInputs.size(); y++)
    {
        for (int x = 0; x < allInputs[y].size(); x++)
        {
            if (allInputs[y][x] == 'S')
            {
                return Coord { x, y };
            }
        }
    }
}

Direction directionFlipper(Direction dir)
{
    if (dir == Direction::EAST) return Direction::WEST;
    if (dir == Direction::WEST) return Direction::EAST;
    if (dir == Direction::NORTH) return Direction::SOUTH;
    if (dir == Direction::SOUTH) return Direction::NORTH;
    return dir;
}

std::pair<Direction,Coord> getStartMove(Coord start, const std::vector<std::vector<char>>& allLines)
{
    //todo range check? shouldnt need to actually
    if (adjacentTileConnects(directionFlipper(Direction::WEST), allLines[start.second][start.first - 1])) return std::pair<Direction, Coord> { Direction::WEST, Coord{ start.first - 1, start.second } };
    if (adjacentTileConnects(directionFlipper(Direction::EAST), allLines[start.second][start.first+1])) return std::pair<Direction, Coord> { Direction::EAST, Coord{   start.first + 1, start.second } };
    if (adjacentTileConnects(directionFlipper(Direction::NORTH), allLines[start.second-1][start.first])) return std::pair<Direction, Coord> { Direction::NORTH, Coord{ start.first, start.second - 1 } };
    if (adjacentTileConnects(directionFlipper(Direction::SOUTH), allLines[start.second+1][start.first])) return std::pair<Direction, Coord> { Direction::SOUTH, Coord{ start.first, start.second + 1 } };
    return std::pair<Direction, Coord> { Direction::NORTH, Coord{ start.first, start.second } };
}

Coord getMove(Direction dir, Coord coord)
{
    Coord newCoord = coord;
    switch (dir)
    {
        case Direction::EAST:
        {
            newCoord.first = newCoord.first + 1;
            break;
        }
        case Direction::WEST:
        {
            newCoord.first = newCoord.first - 1;
            break;
        }
        case Direction::NORTH:
        {
            newCoord.second = newCoord.second - 1;
            break;
        }
        case Direction::SOUTH:
        {
            newCoord.second = newCoord.second + 1;
            break;
        }
        default:
        {

        }
    }
    return newCoord;
}


int Day10::solvePart1()
{
    std::ifstream file("2023/Day10.txt");
    auto allLines = parseInput(file);
    auto startLocation = getStartPair(allLines);

    char newChar = '0';
    std::vector<Coord> allCoords;
    allCoords.push_back(startLocation);
    std::pair<Direction, Coord> startMove = getStartMove(startLocation, allLines);
    auto currentDirection = startMove.first;
    auto currentLocation = startMove.second;
    while(newChar != 'S')
    { 
        allCoords.push_back(currentLocation);
        currentDirection = tilesOtherConnection(allLines[currentLocation.second][currentLocation.first], directionFlipper(currentDirection));
        currentLocation = getMove(currentDirection, Coord{ currentLocation.first , currentLocation.second });
        newChar = allLines[currentLocation.second][currentLocation.first];
    }

    return allCoords.size() / 2; //prompt wants number of furthest search
} 



int Day10::solvePart2()
{
    std::ifstream file("2023/Day10.txt");
    auto allLines = parseInput(file);
    auto startLocation = getStartPair(allLines);

    char newChar = '0';
    std::vector<Coord> allCoords;
    allCoords.push_back(startLocation);
    std::pair<Direction, Coord> startMove = getStartMove(startLocation, allLines);
    auto currentDirection = startMove.first;
    auto currentLocation = startMove.second;
    while (newChar != 'S')
    {
        allCoords.push_back(currentLocation);
        currentDirection = tilesOtherConnection(allLines[currentLocation.second][currentLocation.first], directionFlipper(currentDirection));
        currentLocation = getMove(currentDirection, Coord{ currentLocation.first , currentLocation.second });
        newChar = allLines[currentLocation.second][currentLocation.first];
    }

    std::vector<std::vector<std::pair<int, int>>> boundXs (allLines.size());
    std::vector<std::vector<std::pair<int, int>>> boundYs(allLines[0].size());
    std::vector<int> lastXs = std::vector<int>(allLines.size(), 0);
    std::vector<int> lastYs = std::vector<int>(allLines[0].size(), 0);

    for (auto pair : allCoords)
    {
        if (lastXs[pair.second] != 0)
        {
            boundXs[pair.second].push_back(std::pair<int, int>(pair.first <= lastXs[pair.second] ? pair.first : lastXs[pair.second], pair.first >= lastXs[pair.second] ? pair.first : lastXs[pair.second]));
            lastXs[pair.second] = 0;
        }
        else 
        {
            lastXs[pair.second] = pair.first;
        }
        if (lastYs[pair.first] != 0)
        {
            boundYs[pair.first].push_back(std::pair<int, int>(pair.second <= lastYs[pair.first] ? pair.second : lastYs[pair.first], pair.second >= lastYs[pair.first] ? pair.second : lastYs[pair.first]));
            lastYs[pair.first] = 0;
        }
        else
        {
            lastYs[pair.first] = pair.second;
        }
    }

    std::vector<Coord> boundedCoords;
    for (int y = 0; y < allLines.size(); y++)
    {
        for (int x = 0; x < allLines[0].size(); x++)
        {    
            bool existingCoord = false;
            for (auto coord : allCoords)
            {
                if (coord.first == x && coord.second == y) existingCoord = true;
            }
            if (existingCoord) continue;

            int foundX = 0;
            int foundY = 0;
            for(auto xBound : boundXs[y])
            {
                if (x > xBound.first && x < xBound.second)
                {
                    foundX++;
                }
            }
            for (auto yBound : boundYs[x])
            {
                if (y > yBound.first && y < yBound.second)
                {
                    foundY++;
                }
            }
            if (foundX % 2 == 1 && foundY % 2 == 1) boundedCoords.push_back({ x , y });
        }
    }


    return boundedCoords.size(); 

}

int Day10::solve()
{
    std::cout << "Solving Day 9" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};