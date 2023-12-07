#pragma once
#include "AdventOfCode.h"


int main()
{
    Day* today = new Day1();


    std::cout << "Advent of Code: " << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto success = today->solve();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Duration in ms: " << duration.count() << std::endl;

    return 0;
}






