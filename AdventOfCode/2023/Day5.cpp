#pragma once
#include "Day5.h"

using CategoryTuple = std::tuple<long long, long long, long long>;
using SeedPair = std::pair<long long, long long>;

class Layer
{
public:
    int firstLayerIndex = 0;
    int level = 0;
    long rangeStart = 0;
    long rangeEnd = 0;
};

std::vector<long long> getSeedNumbers(const std::string& seedString)
{
    std::stringstream ss(seedString);
    std::string word;
    std::vector<long long> seeds;
    ss >> word; //skip "seeds"
    while (ss >> word)
    {
        auto seedNumber = std::stoll(word);
        seeds.push_back(seedNumber);
    }
    return seeds;
}

std::vector<std::vector<CategoryTuple>> parseCategories(std::ifstream& file)
{
    std::vector<std::vector<CategoryTuple>> allCategories;
    std::vector<CategoryTuple> category;
    std::string str;

    while (std::getline(file, str))
    {
        std::stringstream ss(str);
        std::string word;
        if (str.empty()) //we finished the category
        {
            std::vector<CategoryTuple> finished(category);
            allCategories.push_back(finished);
            category.clear();
            std::getline(file, str); //skip next category title
            continue;
        }
        ss >> word;
        long long rangeStart = std::stoll(word);
        ss >> word;
        long long seedRangeStart = std::stoll(word);
        ss >> word;
        long long rangeLength = std::stoll(word);
        category.push_back({ rangeStart, seedRangeStart, rangeLength });
    }

    std::vector<CategoryTuple> finished(category);
    allCategories.push_back(finished); //add last category after running out of lines

    return allCategories;
}

long long Day5::solvePart1()
{
    std::vector<long long> seedNumbers;
    std::vector<std::vector<CategoryTuple>> allCategories;

    std::ifstream file("2023/Day5.txt");
    std::string str;
    std::getline(file, str); //first line

    seedNumbers = getSeedNumbers(str);

    std::getline(file, str); //skip blank line after seed numbers
    std::getline(file, str); //skip first title

    allCategories = parseCategories(file);

    long long lowestSeed = LLONG_MAX;
    for (auto seed : seedNumbers) //check each seed number
    {
        for (auto cat : allCategories) //go through each category
        {
            for (auto& ranges : cat) //check each range in category for match
            {
                const auto& [start, seedStart, length] = ranges;
                if (seed >= seedStart && seed <= seedStart + length)
                {
                    seed = start + (seed - seedStart); //adjust seed for new category
                    break;
                }
            }
        }
        if (seed < lowestSeed) lowestSeed = seed; //find the lowest end location
    }
    return lowestSeed;
}

std::vector<SeedPair> parseSeedPairs(const std::string& str)
{
    std::vector<SeedPair> seedPairs;
    std::stringstream ss(str);
    std::string word;
    ss >> word; //skip seeds
    while (ss >> word)
    {
        auto seedNumberStart = std::stoll(word); //first word is startRange
        ss >> word;
        auto seedNumberRange = std::stoll(word); //second word is range
        seedPairs.push_back({ seedNumberStart,seedNumberRange });
    }
    return seedPairs;
}

long long Day5::solvePart2()
{
    //
    //
    // somewhat brutal brute force method because i am busy today. 
    //      Started proper method at end of file and might revisit later 
    //
    //       running time ~10 minutes

    std::vector<SeedPair> seedPairs;
    std::vector<std::vector<CategoryTuple>> allCategories;

    std::ifstream file("2023/Day5.txt");
    std::string str;
    std::getline(file, str); //get first line

    seedPairs = parseSeedPairs(str);
    allCategories = parseCategories(file); 

    //find the lowest end location and check each from the bottom
    auto lastCategory = allCategories.back(); 
    std::sort(lastCategory.begin(), lastCategory.end(), [](auto& left, auto& right) { return std::get<0>(left) < std::get<0>(right); });

    allCategories.pop_back(); //we are starting from lowest category (sorted above)
    std::reverse(allCategories.begin(), allCategories.end()); //parse through in reverse order

    long long currentSeed = 0;
    long long lowest = 0;
    for (auto& lastRange : lastCategory) //check each range in last category (we sorted to start with lowest ranges).
    {
        const auto& [start, seedStart, length] = lastRange;
        for (long long startLocation = seedStart; startLocation < seedStart + length; startLocation++) //check each number (brute force)
        {
            currentSeed = startLocation;
            for (auto cat : allCategories) //go through each category in reverse order (reversed above)
            {
                for (auto& ranges : cat) //check each range for match
                {
                    const auto& [start, seedStart, length] = ranges;
                    if (currentSeed >= start && currentSeed < start + length)
                    {
                        currentSeed = seedStart + (currentSeed - start); //reverse convert per the rules of the question
                        break;
                    }
                }
            }
            //current seed is now top level, check if its in any of our start ranges.
            for (auto seed : seedPairs)
            {
                if (currentSeed >= seed.first && currentSeed <= seed.first + seed.second)
                {
                    return lowest; //we found one! it must be the lowest as we started checking the lowest possible
                }
            }
        }
    }
    return lowest; //fail
}


int Day5::solve()
{
    std::cout << "Solving Day 5" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;


    return 0;
};

//
//
// Start of the non-brute force method, might revisit later
//
//


/*  std::vector<std::shared_ptr<Layer>> allLayers;
  int layerIndex = 1;

  for (auto& range : allFarms[0])
  {
      const auto& [currentStart, currentSeedStart, currentLength] = range;
      auto newLayer = std::make_shared<Layer>();
      newLayer->firstLayerIndex = layerIndex;
      newLayer->level = 1;
      newLayer->rangeStart = currentStart;
      newLayer->rangeEnd = currentSeedStart + currentLength;
      allLayers.push_back(newLayer);
  }

  for (auto layer : allLayers)
  {
      for (auto& ranges : allFarms[1])
      {
          layerIndex++;
          const auto& [start, seedStart, length] = ranges;
          long long startPoint = start;
          long long endPoint = start + length;
          for (int i = 1; i < allFarms.size(); i++)
          {
              auto currentFarm = allFarms[i];
              for (auto& currentRanges : currentFarm)
              {
                  const auto& [currentStart, currentSeedStart, currentLength] = currentRanges;
                  long long currentStartPoint = currentSeedStart;
                  long long currentEndPoint = currentSeedStart + length;
                  if (startPoint >= currentSeedStart && startPoint <= currentEndPoint)
                  {
                      auto newLayer = std::make_shared<Layer>();
                      newLayer->firstLayerIndex = layerIndex;
                      newLayer->level = i;
                      newLayer->rangeStart = startPoint;
                      newLayer->rangeEnd = (endPoint <= currentEndPoint) ? endPoint : currentEndPoint;
                      allLayers.push_back(newLayer);
                  }

                  if (endPoint <= currentEndPoint && endPoint >= currentStartPoint)
                  {
                      auto newLayer = std::make_shared<Layer>();
                      newLayer->firstLayerIndex = layerIndex;
                      newLayer->level = i;
                      newLayer->rangeStart = (startPoint >= currentEndPoint) ? startPoint : currentStartPoint;
                      newLayer->rangeEnd = endPoint;
                      allLayers.push_back(newLayer);
                  }
              }
          }
      }

  }
  */



