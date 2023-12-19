#include "Day14.h"

//
//Again, a few days behind so not optimized.
//
enum class Direction 
{
    NORTH,
    WEST,
    SOUTH,
    EAST
};
 
std::vector<std::string> parseGridInput(std::ifstream& file) //every line in a vector
{
    std::string str;
    std::string word;
    std::vector<std::string> grid;
    while (std::getline(file, str))
    {
        grid.push_back(str);
    }
    return grid;
}

void tiltRocks(std::vector<std::string>& input, Direction dir) //tilt the rocks
{
    std::vector<int> maxes; //the index of the furthest available space to move
    switch (dir) 
    {
        case (Direction::NORTH):
        {
            maxes = std::vector<int> (input.size(), 0); //starting at 0 y-index for each column
            for (int y = 0; y < input.size(); y++) //iterate down, rocks moving up the columns (y indices) if possible
            {
                for (int x = 0; x < input.size(); x++) //left to right (doesnt matter)
                {
                    if (input[y][x] == '.') continue;
                    if (input[y][x] == 'O')
                    {
                        input[y][x] = '.'; //rock moved
                        input[maxes[x]][x] = 'O'; //rock moved to furthest available (might be here still so order matters). 
                        maxes[x] = maxes[x] + 1; //next available is the next row down.
                    }
                    if (input[y][x] == '#')
                    {
                        maxes[x] = y + 1; //rocks cant move past here, we have a new next available for this column
                    }
                }
            }
        }
        break;
        case (Direction::WEST):
        {
            maxes = std::vector<int>(input.size(), 0); 
            for (int y = 0; y < input.size(); y++) //iterate down (doesnt matter)
            {
                for (int x = 0; x < input.size(); x++) //iterate right, rocks move to most leftward available
                {
                    if (input[y][x] == '.') continue;
                    if (input[y][x] == 'O')
                    {
                        input[y][x] = '.';
                        input[y][maxes[y]] = 'O';
                        maxes[y] = maxes[y] + 1;
                    }
                    if (input[y][x] == '#')
                    {
                        maxes[y] = x + 1;
                    }
                }
            }
        }
            break;
        case (Direction::SOUTH):
        {
            maxes = std::vector<int>(input.size(), input.size()-1); //now starting at maximum possible value for each column/row (column in this case)
            for (int y = input.size() - 1; y >= 0; y--) //iterate up, rocks move to most downward possible
            {
                for (int x = 0; x < input.size(); x++) //left to right (doesnt matter)
                {
                    if (input[y][x] == '.') continue;
                    if (input[y][x] == 'O')
                    {
                        input[y][x] = '.';
                        input[maxes[x]][x] = 'O';
                        maxes[x] = maxes[x] - 1;
                    }
                    if (input[y][x] == '#')
                    {
                        maxes[x] = y - 1;
                    }
                }
            }
        }
            break;
        case (Direction::EAST):
            maxes = std::vector<int>(input.size(), input.size()-1); //now starting at maximum possible value for each column/row (row in this case)
            for (int y = 0; y < input.size(); y++) //iterate down (doesnt matter)
            {
                for (int x = input.size() - 1; x >=0; x--) //go right to left, rocks move to most rightward available position
                {
                    if (input[y][x] == '.') continue;
                    if (input[y][x] == 'O')
                    {
                        input[y][x] = '.';
                        input[y][maxes[y]] = 'O';
                        maxes[y] = maxes[y] - 1;
                    }
                    if (input[y][x] == '#')
                    {
                        maxes[y] = x - 1;
                    }
                }
            }
            break;
        default:
            break;
    }
}

int countNorthLoad(std::vector<std::string>& grid) //always count from north side (as prompted by challenge)
{
    int count = 0;
    for (int y = 0; y < grid.size(); y++) //go down 
    {
        for (int x = 0; x < grid.size(); x++) //go right
        {
            if (grid[y][x] == 'O')
            {
                count += grid.size() - y; //load = inverse distance from north side 
            }
        }
    }
    return count;
}


long long Day14::solvePart1()
{
    std::ifstream file("2023/Day14.txt");
    std::vector<std::string> inputGrid = parseGridInput(file);
    tiltRocks(inputGrid, Direction::NORTH);
    return countNorthLoad(inputGrid);
}

long long Day14::solvePart2()
{
    std::ifstream file("2023/Day14.txt");
    std::vector<std::string> inputGrid = parseGridInput(file);

    std::vector<int> pastCounts;
    std::vector<std::vector<std::string>> pastGrids;

    std::vector<int> releveantCounts;

    long long target = 1000000000L;
    int currentIndex = 0;
    int period = 0;
    for (long long i = 0; i < target; i++) //will break long before then
    {
        //tilt in each direction
        tiltRocks(inputGrid, Direction::NORTH);
        tiltRocks(inputGrid, Direction::WEST);
        tiltRocks(inputGrid, Direction::SOUTH);
        tiltRocks(inputGrid, Direction::EAST);

        int count = countNorthLoad(inputGrid); //count after cycle
        if (std::find(pastGrids.begin(), pastGrids.end(), inputGrid) != pastGrids.end()) //look if we have seen this grid before
        {
            //repeat!
            period = i - (std::find(pastGrids.begin(), pastGrids.end(), inputGrid) - pastGrids.begin()); //find the period of the board repeats
            currentIndex = i; //use this to remove initial counts before repeat cycle
            releveantCounts = std::vector<int>(pastCounts.begin() + (currentIndex - period), pastCounts.end()); //only care about grids/counts in the repeat period
            break;
        }
        pastCounts.push_back(count);
        pastGrids.push_back(inputGrid);
        std::cout << i << " " << count << std::endl;
    }
    target = target - currentIndex - 1; //adjust target down to remove boards prior to cycle
    auto spotInPeriod = target % releveantCounts.size(); //find the target cycles position within the cycle.
    return releveantCounts[spotInPeriod];
} 

int Day14::solve()
{
    std::cout << "Solving Day 14" << std::endl;

   auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};