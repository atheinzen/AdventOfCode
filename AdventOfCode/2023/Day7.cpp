#include "Day7.h"

long long FIVE_OF_A_KIND = 80000000000;
long long FOUR_OF_A_KIND = 60000000000;
long long FULL_HOUSE = 40000000000;
long long THREE_OF_A_KIND = 30000000000;
long long TWO_PAIR = 20000000000;
long long PAIR = 10000000000;

int faceCardToInt(char a) //convert face card to decimal value
{
    switch (a) 
    {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;
        default: return 0; //will be ignored
    }
}

std::pair<int, std::vector<int>> cardsToBuckets(std::string& str, std::string& ranker, std::function<int(char)> converter)
{
    std::vector<int> buckets(15,0); //15 ints to 0
    std::string word;
    std::stringstream ss(str);
    ss >> word; //cards
    std::string cards = word;
    ss >> word; // bid
    int bid = std::stoi(word);

    //put each card into a bucket of its own, and, create a string that represents its tiebreak "ranker" i.e. AAAAA -> 1414141414
    for (auto letter : cards) 
    {
        if (std::isdigit(letter)) //number card
        {
            buckets[letter - '0'] += 1; //- '0' convert char to its int value
            ranker += "0" + std::to_string(letter - '0'); //prefix single digit with a 0 to mantain its place
        }
        else //face card
        {
            int letterConverted = converter(letter); //convert to decimal
            buckets[letterConverted] += 1;
            if (letterConverted == 1) //this is for when jack = joker as specified by prompt, 
            {
                ranker += "0" + std::to_string(letterConverted); //jack becomes lowest card "01"
            }
            else
            {
                ranker += std::to_string(letterConverted); //normal
                
            }
        }
    }
    return std::pair<int, std::vector<int>>{bid,buckets};
}

//its really "Hand" type, but prompt calls it class for some reason
long long getClassType(std::vector<int> buckets)
{
    long long classType = 0;
    for (int i : buckets) // 5, 4 1, 3 2, 3, 2 2, 2, 1 1 1 1 1
    {
        if (i >= 5)
        {
            classType = FIVE_OF_A_KIND;
            break; //highest possible
        }
        else if (i == 4)
        {
            classType = FOUR_OF_A_KIND;
            break; //highest possible
        }
        else if (i == 3)         //full house is a combination of three of a kind and pair.
        {
            classType += THREE_OF_A_KIND; //dont break, might be a pair also
        }
        else if (i == 2)         //full house is a combination of three of a kind and pair.
        {
            classType += PAIR; //dont break, might be another pair or three of a kind
        }
    }
    return classType;
}

long long getHandsScore(std::map<long long, int> cardsMap)
{
    int index = 1;
    long long runningTotal = 0;
    for (auto sortedBid : cardsMap)
    {
        runningTotal += sortedBid.second * index++;
    }
    return runningTotal;
}

int Day7::solvePart1()
{
    std::ifstream file("2023/Day7.txt");
    std::string str;
    std::string word;
    std::map<long long, int> cardsMap;
    while (std::getline(file, str))
    {
        std::pair<int, std::vector<int>> bidAndBuckets;
        std::string ranker;
        bidAndBuckets = cardsToBuckets(str, ranker, faceCardToInt);
        long long rank = std::stoll(ranker); //AAAAA -> 1414141414. First card is the tiebreaker, followed by second etc...
        long long classType = getClassType(bidAndBuckets.second); //Five of a kind -> 90000000000
        //large number prefix depending on hand type with rank string for final rank i.e. 91414141414 for 5 of a kind(9) + 5 aces(1414141414).
        long long finalRank = rank + classType; 
        cardsMap[finalRank] = bidAndBuckets.first; //map sorts automatically
    }
    return getHandsScore(cardsMap);
}


int faceCardToIntPart2(char a)
{
    switch (a)
    {
    case 'A': return 14;
    case 'K': return 13;
    case 'Q': return 12;
    case 'J': return 1; //now a joker, and weakest tiebreaker. Bucket 1 previously empty (Ace is high)
    case 'T': return 10;
    default: return 0; //will be ignored
    }
}

void addJokers(std::vector<int>& buckets)
{
    int js = buckets[1]; //joker index
    buckets[1] = 0; //ignore jacks now, it will always be better to use some other value
    if (js > 0)
    {
        //add the joker wild cards to the already most populous card
        *std::max_element(buckets.rbegin(), buckets.rend()) += js; //JJJJJ will be the worst tiebreaker five of a kind
    }
}

int Day7::solvePart2()
{
    std::ifstream file("2023/Day7.txt");
    std::string str;
    std::string word;
    std::map<long long, int> cardsMap;
    while (std::getline(file, str))
    {
        std::pair<int, std::vector<int>> bidAndBuckets;
        std::string ranker;
        bidAndBuckets = cardsToBuckets(str, ranker, faceCardToIntPart2);

        long long rank = std::stoll(ranker);
        addJokers(bidAndBuckets.second); //part 2 twist, jokers are wild, and now lowest tiebreaker
        long long classType = getClassType(bidAndBuckets.second);

        //large number prefix depending on hand type with rank string for final rank i.e. 91414141414 for 5 of a kind(9) + 5 aces(1414141414).
        long long finalRank = rank + classType;
        cardsMap[finalRank] = bidAndBuckets.first; //map sorts automatically
    }
    return getHandsScore(cardsMap);
}

int Day7::solve()
{
    std::cout << "Solving Day 7" << std::endl;

    auto part1 = solvePart1();
    std::cout << "Part 1: " << part1 << std::endl;

    auto part2 = solvePart2();
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
};