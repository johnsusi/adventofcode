#include <fstream>
#include <iostream>
#include <vector>

auto parse(auto &&infile)
{
    auto numbers = std::vector<uint64_t>{};
    for (uint64_t value; infile >> value;)
        numbers.push_back(value);
    return numbers;
}

auto part1(auto numbers)
{
    int result = 0;
    for (int i = 1; i < numbers.size(); ++i)
        if (numbers[i - 1] < numbers[i])
            ++result;
    return result;
}

auto part2(auto numbers)
{
    std::vector<uint64_t> windows;
    for (int i = 0; i < numbers.size() - 2; ++i)
    {
        int window = 0;
        for (int j = 0; j < 3; ++j)
            window += numbers[i + j];
        windows.push_back(window);
    }
    return part1(windows);
}

int main()
{
    auto input = parse(std::ifstream("2021/day01/data.txt"));
    std::cout << part1(input) << std::endl; // 1400
    std::cout << part2(input) << std::endl; // 1429
    return 0;
}