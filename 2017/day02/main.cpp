#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <common/string.hpp>

auto parse(auto infile)
{
    std::vector<std::vector<int>> result;
    for (std::string line; getline(infile, line);)
    {
        std::stringstream parser(line);
        std::vector<int> row;
        for (int value;parser >> value;)
            row.push_back(value);
        result.push_back(row);
    }
    return result;
}

auto part1(auto input)
{
    auto result = 0ull;
    for (auto row : input)
    {
        auto min = *std::min_element(row.begin(), row.end());
        auto max = *std::max_element(row.begin(), row.end());
        result += max - min;
    }
    return result;
}

auto part2(auto input)
{
    auto result = 0ull;
    for (auto row : input)
    {
        for (int i = 0, n = row.size();i < n;++i)
            for (int j = i + 1;j < n;++j)
                if (row[i] < row[j] && (row[j] % row[i]) == 0)
                    result += row[j] / row[i];
                else if (row[j] < row[i] && (row[i] % row[j]) == 0)
                    result += row[i] / row[j];
    }
    return result;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl;
    std::cout << part2(input) << std::endl;
}