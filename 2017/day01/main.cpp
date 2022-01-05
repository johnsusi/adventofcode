#include <fstream>
#include <iostream>

#include <common/string.hpp>

auto parse(auto infile)
{
    std::string line;
    getline(infile, line);
    return line;
}

auto part1(auto input)
{
    auto sum = 0ull;
    for (int i = 0, n = input.size();i < n;++i)
        if (input[i] == input[(i+1) % n])
            sum += input[i] - '0';
    return sum;
}

auto part2(auto input)
{
    auto sum = 0ull;
    for (int i = 0, n = input.size();i < n;++i)
        if (input[i] == input[(i+n/2) % n])
            sum += input[i] - '0';
    return sum;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl;
    std::cout << part2(input) << std::endl;
}