#include <fstream>
#include <vector>
#include <iostream>

auto part1(std::vector<uint64_t> numbers)
{
    int result = 0;
    for (int i = 1; i < numbers.size(); ++i)
        if (numbers[i-1] < numbers[i])
            ++result;
    return result;
}

auto part2(std::vector<uint64_t> numbers)
{
    std::vector<uint64_t> windows;
    for (int i = 0;i < numbers.size()-2;++i)
    {
        int window = 0;
        for (int j = 0;j < 3;++j)
            window += numbers[i + j];
        windows.push_back(window);
    }
    return part1(windows);
}

int main()
{
    std::ifstream infile("data.txt");

    std::vector<uint64_t> numbers;
    for (uint64_t value; infile >> value;) numbers.push_back(value);

    std::cout << part1(numbers) << std::endl;
    std::cout << part2(numbers) << std::endl;

    return 0;
}