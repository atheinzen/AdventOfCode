#include "Day10.h"

using Coord = std::pair<int, int>;

//Coordinate Enum
enum class Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

//from the perspective of the tile, which other direction do i connect to?
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
        return Direction::SOUTH; //todo null
    }
    default:
        return Direction::SOUTH; //todo null
    }
}

//from the perspective of the starting tile, check which way we can go from the starting tile by checking adjacent tiles for a conneciton. 
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

//2D vectors of chars 
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

//where is start start coordinate? find the 'S'
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

//so we can ask adjacent tiles if they connect based on current tiles directions
Direction directionFlipper(Direction dir)
{
    if (dir == Direction::EAST) return Direction::WEST;
    if (dir == Direction::WEST) return Direction::EAST;
    if (dir == Direction::NORTH) return Direction::SOUTH;
    if (dir == Direction::SOUTH) return Direction::NORTH;
    return dir;
}

//find a connecting tile for the start tile
std::pair<Direction,Coord> getStartMove(Coord start, const std::vector<std::vector<char>>& allLines)
{
    //range check and then check if adjacent tile is a potential first move 
    if (start.first > 0 &&                    adjacentTileConnects(directionFlipper(Direction::WEST), allLines[start.second][start.first - 1])) return std::pair<Direction, Coord> { Direction::WEST, Coord{ start.first - 1, start.second }};
    if (start.first < allLines[0].size()-1 && adjacentTileConnects(directionFlipper(Direction::EAST), allLines[start.second][start.first + 1])) return std::pair<Direction, Coord> { Direction::EAST, Coord{ start.first + 1, start.second }};
    if (start.second > 0 &&                   adjacentTileConnects(directionFlipper(Direction::NORTH), allLines[start.second - 1][start.first])) return std::pair<Direction, Coord> { Direction::NORTH, Coord{ start.first, start.second - 1 }};
    if (start.second < allLines.size()-1 &&   adjacentTileConnects(directionFlipper(Direction::SOUTH), allLines[start.second + 1][start.first])) return std::pair<Direction, Coord> { Direction::SOUTH, Coord{ start.first, start.second + 1 }};
    return std::pair<Direction, Coord> { Direction::NORTH, Coord{ start.first, start.second } }; //shoulnd't happen 
}

//advance the coordinate based on direction of move
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

//traverse the puzzle loop until we get back to 'S'
void getAllPathCoords(std::pair<Direction, Coord> startMove, std::vector<std::vector<char>> allLines, std::vector<Coord>& allCoords)
{
    char newChar = '0';
    auto currentDirection = startMove.first;
    auto currentLocation = startMove.second;
    while (newChar != 'S')
    {
        allCoords.push_back(currentLocation);
        currentDirection = tilesOtherConnection(allLines[currentLocation.second][currentLocation.first], directionFlipper(currentDirection));
        currentLocation = getMove(currentDirection, Coord{ currentLocation.first , currentLocation.second });
        newChar = allLines[currentLocation.second][currentLocation.first];
    }
}

int Day10::solvePart1()
{
    std::ifstream file("2023/Day10.txt");
    auto allLines = parseInput(file); //load in puzzle input
    auto startLocation = getStartPair(allLines); //find the S
    std::pair<Direction, Coord> startMove = getStartMove(startLocation, allLines); //find a possible starting move
    std::vector<Coord> allCoords; 
    allCoords.push_back(startLocation); //plug in 'S' location to start
    getAllPathCoords(startMove, allLines, allCoords); //traverse the loop
    return allCoords.size() / 2; //prompt wants travel distance to furthest point in loop. 
} 

//check if the coordinate we are looking at is in the path.
bool checkExistingCoord(std::pair<int, int> coord, std::vector<Coord> allCoords)
{
    bool existingCoord = false;
    for (auto exostingCoord : allCoords)
    {
        if (exostingCoord.first == coord.first && exostingCoord.second == coord.second) existingCoord = true;
    }
    return existingCoord;
}

//check how many times the path crosses a specific row or column. This will determine if any tile bounded by the loop is in or out. 
void getLineCrossIndices(std::vector<std::vector<int>>& lineCrosses, std::vector<Coord> allCoords, bool horizontal)
{
    for (int x = 0; x < lineCrosses.size(); x++) //for each row or column
    {
        bool above = false; //are we greater than or less than the current row or column
        Coord firstCoord = allCoords[0];
        auto currentSpot = horizontal ? firstCoord.second : firstCoord.first; //our current index, horizontal is rows or columns switch.
        above = currentSpot > x; //we wont really know until we move
        bool onTheLineToStart = (currentSpot == x); //if we start by moving laterally we want to ignore that. 
        for (int i = 1; i < allCoords.size(); i++)
        {
            Coord thisCoord = allCoords[i];
            currentSpot = horizontal ? thisCoord.second : thisCoord.first;
            if (onTheLineToStart && currentSpot != x) //we were moving laterally at the start so we dont count that as a crossing. 
            {
                above = currentSpot > x; //this will now be correct. 
                onTheLineToStart = false;
                continue;
            }
            if ((currentSpot >= x && above) || (currentSpot <= x && !above)) { continue; } //not a transition of the line
            if ((currentSpot > x && !above) || (currentSpot < x && above)) //crossed the line
            {
                if (horizontal) //row or column
                {
                    lineCrosses[x].push_back(thisCoord.first);
                }
                else
                {
                    lineCrosses[x].push_back(thisCoord.second);
                }
                above = currentSpot > x; //swapped
            }
        }
    }
}

//for each point, check how many times we crossed a boundary in both x and y directions before reaching the point. If both are odd, then we are bounded by the loop.
std::vector<Coord> checkLineCrossesForPoints(const std::vector<std::vector<char>>& allLines, std::vector<Coord> allCoords, std::vector<std::vector<int>>& lineCrossXs, std::vector<std::vector<int>>& lineCrossYs)
{
    std::vector<Coord> boundedCoords;
    for (int y = 0; y < allLines.size(); y++) //for each row
    {
        for (int x = 0; x < allLines[0].size(); x++) //for each column
        {
            if (checkExistingCoord({ x,y }, allCoords)) continue; //points that are part of the loop dont count
            int xLineCrosses = 0;
            int yLineCrosses = 0;
            Coord lastCoord;
            for (auto xCoord : lineCrossXs[y]) //row crossing
            {
                if (xCoord < x) 
                {
                    xLineCrosses++;
                }
            }
            for (auto yCoord : lineCrossYs[x]) //column cross really
            {
                if (yCoord < y)
                {
                    yLineCrosses++;
                }
            }
            //odd number of row and column crosses = in the loop. 
            if (xLineCrosses % 2 == 1 && yLineCrosses % 2 == 1) boundedCoords.push_back({ x , y });
        }
    }
    return boundedCoords;
}

int Day10::solvePart2()
{
    std::ifstream file("2023/Day10.txt");
    auto allLines = parseInput(file); //2D vector of chars
    auto startLocation = getStartPair(allLines); //where is the 'S'
    std::pair<Direction, Coord> startMove = getStartMove(startLocation, allLines); //find a possible start move
    std::vector<Coord> allCoords;
    allCoords.push_back(startLocation); //add 'S' as starting point
    getAllPathCoords(startMove, allLines, allCoords); //fill allCoords by traversing the loop

    std::vector<std::vector<int>> importantXs(allLines.size()); //line cross indexes
    std::vector<std::vector<int>> importantYs(allLines[0].size()); //column cross indexes

    getLineCrossIndices(importantXs, allCoords, true); //true = horizontal (row check)
    getLineCrossIndices(importantYs, allCoords, false); //false = vertical (column check);

    std::vector<Coord> boundedCoords = checkLineCrossesForPoints(allLines, allCoords, importantXs, importantYs); //how many tiles in the loop by checking boundary crosses 

    return boundedCoords.size(); //all tiles contained in the loop.
}

int Day10::solve()
{
    std::cout << "Solving Day 10" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};