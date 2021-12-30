#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

auto parse(auto &&infile)
{
    auto result = std::set<std::tuple<int, int, int>>{};
    int y = 0;
    for (std::string line; std::getline(infile, line); ++y)
        for (int x = 0; x < line.size(); ++x)
            if (line[x] == '#')
                result.emplace(0, x, y);
    return result;
}

auto rate(auto map)
{
    auto result = 0ull;
    for (auto [layer, x, y] : map)
        if (map.contains({0, x, y}))
            result += (1 << (x + y * 5));
    return result;
}

auto part1(auto map)
{
    auto visited = std::set<std::set<std::tuple<int, int, int>>>{map};
    while (true)
    {
        auto temp = std::map<std::tuple<int, int, int>, int>{};
        for (auto [layer, x, y] : map)
        {
            if (x > 0)
                temp[{0, x - 1, y}]++;
            if (x < 4)
                temp[{0, x + 1, y}]++;
            if (y > 0)
                temp[{0, x, y - 1}]++;
            if (y < 4)
                temp[{0, x, y + 1}]++;
        }
        auto next = std::set<std::tuple<int, int, int>>{};
        for (auto [pos, value] : temp)
            if (value == 1 || (value == 2 && !map.contains(pos)))
                next.insert(pos);

        if (visited.contains(next))
            return rate(next);
        visited.insert(next);
        map = next;
    }
}

auto part2(auto input)
{
    for (int i = 0; i < 200;++i)
    {
        auto temp = std::map<std::tuple<int, int, int>, int>{};
        for (auto [layer, x, y] : input)
        {
            if (x == 1 && y == 2)
                for (int j = 0; j < 5; ++j)
                    temp[{layer + 1, 0, j}]++;
            else if (x == 2 && y == 1)
                for (int j = 0; j < 5; ++j)
                    temp[{layer + 1, j, 0}]++;
            else if (x == 3 && y == 2)
                for (int j = 0; j < 5; ++j)
                    temp[{layer + 1, 4, j}]++;
            else if (x == 2 && y == 3)
                for (int j = 0; j < 5; ++j)
                    temp[{layer + 1, j, 4}]++;

                if (x == 3 && y == 2)
                    ;
                else if (x > 0)
                    temp[{layer, x - 1, y}]++;
                else
                    temp[{layer - 1, 1, 2}]++;
                if (x == 1 && y == 2)
                    ;
                else if (x < 4)
                    temp[{layer, x + 1, y}]++;
                else
                    temp[{layer - 1, 3, 2}]++;
                if (x == 2 && y == 3)
                    ;
                else if (y > 0)
                    temp[{layer, x, y - 1}]++;
                else
                    temp[{layer - 1, 2, 1}]++;
                if (x == 2 && y == 1)
                    ;
                else if (y < 4)
                    temp[{layer, x, y + 1}]++;
                else
                    temp[{layer - 1, 2, 3}]++;

        }
        auto next = std::set<std::tuple<int, int, int>>{};
        for (auto [pos, value] : temp)
            if (value == 1 || (value == 2 && !input.contains(pos)))
                next.insert(pos);
        input = next;
    }
    return input.size();
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 18400821
    std::cout << part2(input) << std::endl; // 1914
    return 0;
}
