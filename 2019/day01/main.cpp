#include <fstream>
#include <iostream>
#include <vector>

auto part1(const std::vector<int>& modules)
{
    int result = 0;
    for (auto module : modules)
        result += module / 3 - 2;
    return result;
}

auto part2(const std::vector<int>& modules)
{
    int result = 0;
    for (auto module : modules)
    {
        int fuel = module / 3 - 2;
        result += fuel;
        while (fuel > 0)
        {
            fuel = fuel / 3 - 2;
            if (fuel < 0) fuel = 0;
            result += fuel;
        }
    }
    return result;
}


int main()
{
    std::ifstream infile("data.txt");
    std::vector<int> modules;
    for (int module;infile >> module;)
        modules.push_back(module);
    std::cout << part1(modules) << std::endl; // 3576689
    std::cout << part2(modules) << std::endl; // 5362136
    return 0;
}
