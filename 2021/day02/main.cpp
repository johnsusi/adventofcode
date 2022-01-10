#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto parse(auto infile)
{
    std::vector<std::pair<std::string, int>> result;
    int value;
    for (std::string command; infile >> command >> value;)
        result.emplace_back(command, value);
    return result;
}

auto part1(auto input)
{
    auto depth = 0ll, horizontal = 0ll;
    for (auto [command, value] : input)
    {
        if (command == "up")
            depth -= value;
        else if (command == "down")
            depth += value;
        else if (command == "forward")
            horizontal += value;
    }
    return horizontal * depth;
}

auto part2(auto input)
{
    auto depth = 0ll, horizontal = 0ll, aim = 0ll;
    for (auto [command, value] : input)
    {
        if (command == "up")
            aim -= value;
        else if (command == "down")
            aim += value;
        else if (command == "forward")
        {
            depth += aim * value;
            horizontal += value;
        }
    }
    return horizontal * depth;
}

int main()
{
    auto input = parse(std::ifstream("2021/day02/data.txt"));
    std::cout << part1(input) << std::endl; // 1840243
    std::cout << part2(input) << std::endl; // 1727785422
    return 0;
}