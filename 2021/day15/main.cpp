#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <common/viz.hpp>

auto parse(auto& infile)
{
    Map map;
    int y = 0;
    for (std::string line; std::getline(infile, line); ++y)
        for (int x = 0; x < line.size(); ++x) map[{x, y}] = line[x] - '0';
    return map;
}

template <typename T> auto make_queue(T root)
{
    auto queue = std::priority_queue<T, std::vector<T>, std::greater<T>>();
    queue.push(root);
    return queue;
}

auto search(const auto& map)
{
    const auto left  = std::pair{-1, 0};
    const auto right = std::pair{1, 0};
    const auto up    = std::pair{0, -1};
    const auto down  = std::pair{0, 1};

    auto [x1, y1, x2, y2] = map.bounds();
    auto fringe           = make_queue(std::pair{-map[{x1, y1}], std::pair{x1, y1}});
    std::set<std::pair<int, int>> visited;
    auto best = std::numeric_limits<int>::max();
    while (!fringe.empty()) {
        auto [cost, pos] = fringe.top();
        fringe.pop();
        if (!map.contains(pos) || visited.contains(pos)) continue;
        visited.insert(pos);
        cost += map[pos];
        if (cost >= best) continue;
        if (pos == std::pair{x2, y2}) { best = std::min(cost, best); }
        else {
            fringe.emplace(cost, pos + right);
            fringe.emplace(cost, pos + down);
            fringe.emplace(cost, pos + left);
            fringe.emplace(cost, pos + up);
        }
    }
    return best;
}

auto part1(auto map)
{
    return search(map);
}

auto part2(auto map)
{
    auto [x1, y1, x2, y2] = map.bounds();
    auto w                = x2 - x1 + 1;
    auto h                = y2 - y1 + 1;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            for (int y = y1; y <= y2; ++y)
                for (int x = x1; x <= x2; ++x) {
                    auto risk_level = map[{x, y}] + i + j;
                    if (risk_level > 9) risk_level -= 9;
                    map[{x + j * w, y + i * h}] = risk_level;
                }
    return search(map);
}

int main()
{
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 398
    std::cout << part2(input) << std::endl; // 2817
    return 0;
}
