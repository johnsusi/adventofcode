#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>

using tile_t = std::pair<int, int>;

auto is_even = [](auto value) {
    const auto& [_, count] = value;
    return count % 2 == 0;
};
auto is_odd = std::not_fn(is_even);

std::map<std::string, std::pair<int, int>> directions = {{"e", {1, 1}},
                                                         {"se", {0, 1}},
                                                         {"sw", {-1, 0}},
                                                         {"w", {-1, -1}},
                                                         {"nw", {0, -1}},
                                                         {"ne", {1, 0}}};

auto locate(const std::string& path)
{
    int x = 0, y = 0;
    std::string token;
    for (auto c : path) {
        token += c;
        if (directions.contains(token)) {
            auto [dx, dy] = directions[token];
            x += dx;
            y += dy;
            token = {};
        }
    }
    return std::make_pair(x, y);
}

int part1(const std::map<tile_t, int>& tiles)
{
    return std::count_if(tiles.begin(), tiles.end(), is_odd);
}

int part2(std::map<tile_t, int> tiles)
{
    std::erase_if(tiles, is_even);
    for (int i = 0; i < 100; ++i) {
        decltype(tiles) candidates;
        for (auto& [_, count] : tiles) count = 0;
        for (auto [tile, _] : tiles) {
            auto [x, y] = tile;
            for (auto [k, v] : directions) {
                auto [dx, dy]  = v;
                auto candidate = std::make_pair(x + dx, y + dy);
                if (tiles.contains(candidate))
                    ++tiles[candidate];
                else
                    ++candidates[candidate];
            }
        }
        std::erase_if(tiles, [](auto v) {
            auto [_, count] = v;
            return count == 0 || count > 2;
        });
        for (auto [tile, count] : candidates)
            if (count == 2) tiles[tile] = 0;
    }
    return tiles.size();
}

int main()
{

    std::map<tile_t, int> data;

    std::ifstream infile("data.txt");
    for (std::string line; std::getline(infile, line);) ++data[locate(line)];

    std::cout << part1(data) << std::endl; // 512
    std::cout << part2(data) << std::endl; // 4120

    return 0;
}
