#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

auto parse(auto infile)
{
    std::set<std::tuple<int, int, int, int>> data;
    int y = 0;
    for (std::string line; std::getline(infile, line);++y)
        for (int x = 0;x < line.size();++x)
            if (line[x] == '#')
                data.insert({x, y, 0, 0});
    return data;
}

auto simulate(const auto& input, const auto& neighbours)
{
    std::map<std::tuple<int, int, int, int>, int> counts;
    for (const auto& pos : input)
    {
        auto [x, y, z, w] = pos;
        for (auto [i, j, k, l] : neighbours)
            counts[{x+i, y+j, z+k, w+l}]++;
    }
    auto temp = decltype(input){};
    for (const auto& [pos, count] : counts)
        if (count == 3 || (count == 2 && input.contains(pos)))
            temp.insert(pos);
    return temp;
}

auto neighbours(int dx, int dy, int dz, int dw = 0)
{
    std::vector<std::tuple<int, int, int, int>> result;
    for (int i = -dx, o = 0;i <= dx;++i)
        for (int j = -dy;j <= dy;++j)
            for (int k = -dz;k <= dz;++k)
                for (int l = -dw;l <= dw; ++l)
                    if (i == 0 && j == 0 && k == 0 && l == 0) continue;
                    else result[o++] = std::tuple { i, j, k, l};
    return result;
}

auto part1(auto input)
{
    auto n = neighbours(1, 1, 1);
    for (int i = 0;i < 6;++i)
        input = simulate(input, n);
    return input.size();
}

auto part2(auto input)
{
    auto n = neighbours(1, 1, 1, 1);
    for (int i = 0;i < 6;++i)
        input = simulate(input, n);
    return input.size();
}
int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 304
    std::cout << part2(input) << std::endl; // 1868
    return 0;
}
