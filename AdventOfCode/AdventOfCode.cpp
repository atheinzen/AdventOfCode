#pragma once
#include "AdventOfCode.h"


int main()
{
    //Day* today = new Day1();
    //Day* today = new Day2();
    //Day* today = new Day3();
    //Day* today = new Day4();
    //Day* today = new Day5();
    //Day* today = new Day6();
    //Day* today = new Day7();
    //Day* today = new Day8();
    //Day* today = new Day9();
    //Day* today = new Day10();
    //Day* today = new Day11();
    Day* today = new Day12();





    std::cout << "Advent of Code: " << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto success = today->solve();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Duration in ms: " << duration.count() << std::endl;

    return 0;
}






