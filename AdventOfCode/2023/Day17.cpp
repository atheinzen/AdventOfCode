#include "Day17.h"

//
// Not optimized, im behind!!
//
using Coord = std::pair<int, int>;
using Head = std::tuple<Coord, int, std::string>;
using Grid = std::vector<std::vector<int>>;
using GridMap = std::map<std::string, Grid>;

Grid parseDay17Input(std::ifstream& file) //a string for each position in the grid.
{
    std::string str;
    Grid gridOfHeatAndLeast; //each coordinate with a pairing representing their given heat, and eventually their shortest path.
    std::vector<int> thisLine;
    while (std::getline(file, str))
    {
        for (auto c : str)
        {
            std::string s{ c };
            thisLine.push_back(std::stoi(s)); //were gonna make each character a string and then use the string to store the data we need.
        }
        gridOfHeatAndLeast.push_back(thisLine);
        thisLine.clear();
    }
    return gridOfHeatAndLeast;
}

bool outOfBoundsCoord(Coord coord, int maxSize) //assumes a square grid
{
    if (coord.first < 0 || coord.first >= maxSize || coord.second < 0 || coord.second >= maxSize)
    {
        return true;
    }
    return false;
}

void advanceHeads(Grid& grid, const Head& startingHead, GridMap& gridMap)
{
    std::vector<Head> currentHeads;
    std::vector<Head> nextHeads;
	currentHeads.push_back(startingHead);
    while (!currentHeads.empty()) //while we still have heads of the beam we continue
    {
        for (auto head : currentHeads)
        {
            const auto& [coords, totalHeat, consecutiveMovesInThisDirection] = head;

            if (outOfBoundsCoord(coords, grid.size())) //kill off heads that have met their end
            {
                //remove head
                continue;
            }
            if (consecutiveMovesInThisDirection == "EEEE" || consecutiveMovesInThisDirection == "WWWW" ||
                consecutiveMovesInThisDirection == "NNNN" || consecutiveMovesInThisDirection == "SSSS") // We can not continue this one (rules)
            {
                //remove head
                continue;
            }
            std::string recentString;
            for (char c : consecutiveMovesInThisDirection) {
                if (recentString.empty())
                {
                    recentString += c;
                    continue;
                }
                if (c == recentString[0]) recentString += c;
                else
                {
                    break;
                }
            }

            auto thisHeat = grid[coords.second][coords.first];
            auto previousLowHeatForHere = gridMap[recentString][coords.second][coords.first];

            auto newTotalHeatForThisSpot = thisHeat + totalHeat;
            if (newTotalHeatForThisSpot >= previousLowHeatForHere)
            {
                //remove head, not viable to be new low
                continue;
            }
            gridMap[recentString][coords.second][coords.first] = newTotalHeatForThisSpot;

            //can not go backwards...
            if (recentString[0] != 'E') nextHeads.push_back({{ coords.first - 1 , coords.second }, newTotalHeatForThisSpot, "W" + recentString});
            if (recentString[0] != 'W') nextHeads.push_back({ { coords.first + 1 , coords.second }, newTotalHeatForThisSpot, "E" + recentString});
            if (recentString[0] != 'N') nextHeads.push_back({ { coords.first, coords.second + 1 }, newTotalHeatForThisSpot, "S" + recentString});
            if (recentString[0] != 'S') nextHeads.push_back({ { coords.first, coords.second - 1}, newTotalHeatForThisSpot, "N" + recentString});
        }
        currentHeads = nextHeads;
        nextHeads.clear();
    }
}


void advanceHeadsPart2(Grid& grid, const Head& startingHead, GridMap& gridMap)
{
    std::vector<Head> currentHeads;
    std::vector<Head> nextHeads;
    currentHeads.push_back(startingHead);
    while (!currentHeads.empty()) //while we still have heads of the beam we continue
    {
        for (auto head : currentHeads)
        {
            const auto& [coords, totalHeat, consecutiveMovesInThisDirection] = head;

            if (outOfBoundsCoord(coords, grid.size())) //kill off heads that have met their end
            {
                //remove head
                continue;
            }
            if (consecutiveMovesInThisDirection == "EEEEEEEEEEE" || consecutiveMovesInThisDirection == "WWWWWWWWWWW" ||
                consecutiveMovesInThisDirection == "NNNNNNNNNNN" || consecutiveMovesInThisDirection == "SSSSSSSSSSS") // We can not continue this one (rules)
            {
                //remove head
                continue;
            }

            //check how many consecutive moves in this direction
            std::string recentString;
            for (char c : consecutiveMovesInThisDirection) {
                if (recentString.empty())
                {
                    recentString += c;
                    continue;
                }
                if (c == recentString[0]) recentString += c;
                else
                {
                    break;
                }
            }

            auto thisHeat = grid[coords.second][coords.first];
            auto newTotalHeatForThisSpot = thisHeat + totalHeat;
            if (recentString.length() < 4) //send through, not a new low reached here as not valid (rules of the problem)
            {
                //cannot reverse per rules
                if (recentString[0] == 'W') nextHeads.push_back({ { coords.first - 1 , coords.second }, newTotalHeatForThisSpot, "W" + recentString });
                else if (recentString[0] == 'E') nextHeads.push_back({ { coords.first + 1 , coords.second }, newTotalHeatForThisSpot, "E" + recentString });
                else if (recentString[0] == 'S') nextHeads.push_back({ { coords.first, coords.second + 1 }, newTotalHeatForThisSpot, "S" + recentString });
                else if (recentString[0] == 'N') nextHeads.push_back({ { coords.first, coords.second - 1}, newTotalHeatForThisSpot, "N" + recentString });
                continue;
            }

            auto previousLowHeatForHere = gridMap[recentString][coords.second][coords.first];
            if (newTotalHeatForThisSpot >= previousLowHeatForHere)
            {
                //remove head
                continue;
            }
            gridMap[recentString][coords.second][coords.first] = newTotalHeatForThisSpot;

            if (recentString[0] != 'E') nextHeads.push_back({ { coords.first - 1 , coords.second }, newTotalHeatForThisSpot, "W" + recentString });
            if (recentString[0] != 'W') nextHeads.push_back({ { coords.first + 1 , coords.second }, newTotalHeatForThisSpot, "E" + recentString });
            if (recentString[0] != 'N') nextHeads.push_back({ { coords.first, coords.second + 1 }, newTotalHeatForThisSpot, "S" + recentString });
            if (recentString[0] != 'S') nextHeads.push_back({ { coords.first, coords.second - 1}, newTotalHeatForThisSpot, "N" + recentString });
        }
        currentHeads = nextHeads;
        nextHeads.clear();
    }
}

long long Day17::solvePart1()
{
    std::ifstream file("2023/Day17.txt");
    Grid grid = parseDay17Input(file);
    Grid maxGrid(grid.size(), std::vector<int>(grid.size(), std::numeric_limits<int>::max()));
    GridMap gridMap;
    gridMap["N"] = maxGrid;
    gridMap["NN"] = maxGrid;
    gridMap["NNN"] = maxGrid;
    gridMap["E"] = maxGrid;
    gridMap["EE"] = maxGrid;
    gridMap["EEE"] = maxGrid;
    gridMap["S"] = maxGrid;
    gridMap["SS"] = maxGrid;
    gridMap["SSS"] = maxGrid;
    gridMap["W"] = maxGrid;
    gridMap["WW"] = maxGrid;
    gridMap["WWW"] = maxGrid;


	std::vector<Head> startingHeads; // x,y currentHeatLoss, consecutiveMoves
    startingHeads.push_back({ { 1, 0 }, 0, "E"});
    startingHeads.push_back({ { 0, 1 }, 0, "S" });
    for (auto startingHead : startingHeads) //run the process for all possible starting directions.
    {
        advanceHeads(grid, startingHead, gridMap);
    }
    int least = std::numeric_limits<int>::max();
    for (const auto& [key, value] : gridMap)
    {
	    if(value[grid.size()-1][grid.size()-1] < least)
	    {
            least = value[grid.size() - 1][grid.size() - 1];
	    }
    }
    return least;
}

long long Day17::solvePart2()
{
    std::ifstream file("2023/Day17.txt");
    Grid grid = parseDay17Input(file);
    Grid maxGrid(grid.size(), std::vector<int>(grid.size(), std::numeric_limits<int>::max()));

    //brute force
    GridMap gridMap;
    gridMap["NNNN"] = maxGrid;
    gridMap["NNNNN"] = maxGrid;
    gridMap["NNNNNN"] = maxGrid;
    gridMap["NNNNNNN"] = maxGrid;
    gridMap["NNNNNNNN"] = maxGrid;
    gridMap["NNNNNNNNN"] = maxGrid;
    gridMap["NNNNNNNNNN"] = maxGrid;
    gridMap["SSSS"] = maxGrid;
    gridMap["SSSSS"] = maxGrid;
    gridMap["SSSSSS"] = maxGrid;
    gridMap["SSSSSSS"] = maxGrid;
    gridMap["SSSSSSSS"] = maxGrid;
    gridMap["SSSSSSSSS"] = maxGrid;
    gridMap["SSSSSSSSSS"] = maxGrid;
	gridMap["EEEE"] = maxGrid;
    gridMap["EEEEE"] = maxGrid;
    gridMap["EEEEEE"] = maxGrid;
    gridMap["EEEEEEE"] = maxGrid;
    gridMap["EEEEEEEE"] = maxGrid;
    gridMap["EEEEEEEEE"] = maxGrid;
    gridMap["EEEEEEEEEE"] = maxGrid;
	gridMap["WWWW"] = maxGrid;
    gridMap["WWWWW"] = maxGrid;
    gridMap["WWWWWW"] = maxGrid;
    gridMap["WWWWWWW"] = maxGrid;
    gridMap["WWWWWWWW"] = maxGrid;
    gridMap["WWWWWWWWW"] = maxGrid;
    gridMap["WWWWWWWWWW"] = maxGrid;


    std::vector<Head> startingHeads; // x,y currentHeatLoss, consecutiveMoves
    startingHeads.push_back({ { 1, 0 }, 0, "E" });
    startingHeads.push_back({ { 0, 1 }, 0, "S" });
    for (auto startingHead : startingHeads) //run the process for all possible starting directions.
    {
        advanceHeadsPart2(grid, startingHead, gridMap);
    }
    int least = std::numeric_limits<int>::max();
    for (const auto& [key, value] : gridMap)
    {
        if (value[grid.size() - 1][grid.size() - 1] < least)
        {
            least = value[grid.size() - 1][grid.size() - 1];
        }
    }
    return least;
}

int Day17::solve()
{
    std::cout << "Solving Day 17" << std::endl;

    auto part1 = Day17::solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = Day17::solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};
