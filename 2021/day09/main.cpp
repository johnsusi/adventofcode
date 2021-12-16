#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto infile) {
    std::vector<std::string> map;
    for (std::string line; std::getline(infile, line);) map.push_back(line);
    return map;
}

auto search(const std::vector<std::string>& map)
{
    int w       = map.front().size();
    int h       = map.size();
    auto result = std::vector<std::pair<int, int>>{};
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if ((x == 0 || (map[y][x] < map[y][x - 1])) &&
                (y == 0 || (map[y][x] < map[y - 1][x])) &&
                (x == w - 1 || (map[y][x] < map[y][x + 1])) &&
                (y == h - 1 || (map[y][x] < map[y + 1][x])))
                result.push_back({x, y});
        }
    }
    return result;
}

auto part1(const std::vector<std::string>& map)
{
    int result = 0;
    for (auto [x, y] : search(map)) result += 1 + (int)(map[y][x] - '0');
    return result;
}

auto part2(std::vector<std::string> map)
{
    int w = map.front().size();
    int h = map.size();
    std::vector<int> basins;
    for (auto [x0, y0] : search(map)) {
        int basin   = 0;
        auto fringe = std::vector{std::pair{x0, y0}};
        while (!fringe.empty()) {
            auto [x, y] = fringe.back();
            fringe.pop_back();
            if (x < 0 || x >= w || y < 0 || y >= h || map[y][x] == '9') continue;
            map[y][x] = '9';
            basin++;
            fringe.push_back({x - 1, y});
            fringe.push_back({x, y - 1});
            fringe.push_back({x + 1, y});
            fringe.push_back({x, y + 1});
        }
        basins.push_back(basin);
    }
    assert(basins.size() >= 3);
    std::sort(basins.begin(), basins.end());
    return std::accumulate(basins.end() - 3, basins.end(), 1, std::multiplies());
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 491
    std::cout << part2(input) << std::endl; // 1075536
    return 0;
}
