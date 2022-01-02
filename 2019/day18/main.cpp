#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>

using Position = std::pair<int, int>;
const auto left = Position{-1, 0};
const auto right = Position{1, 0};
const auto up = Position{0, -1};
const auto down = Position{0, 1};

template <typename T> auto operator+(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs)
{
    return std::pair{lhs.first + rhs.first, lhs.second + rhs.second};
}

auto search(const auto &map, auto start, const auto &state) -> decltype(0ull)
{
    static std::map<std::pair<decltype(start), std::set<char>>, decltype(0ull)> cache;
    auto fringe = std::deque{std::tuple{0ull, start}};
    auto result = std::numeric_limits<decltype(0ull)>::max();
    std::set<Position> visited;
    while (!fringe.empty())
    {
        auto [cost, positions] = fringe.front();
        fringe.pop_front();
        if (cost >= result)
            continue;
        for (int i = 0; i < positions.size(); ++i)
        {
            auto pos = positions[i];
            if (visited.contains(pos))
                continue;
            visited.insert(pos);
            auto ch = map.at(pos);
            if (ch == '#')
                continue;
            else if (ch >= 'A' && ch <= 'Z' && state.contains(std::tolower(ch)))
                continue;
            else if (ch >= 'a' && ch <= 'z' && state.contains(ch))
            {
                auto s = state;
                s.erase(ch);
                if (cache.contains({positions, s}))
                    cost += cache.at({positions, s});
                else if (!s.empty())
                    cost += cache[{positions, s}] = search(map, positions, s);
                result = std::min(result, cost);
            }
            else
            {
                auto p = positions;
                for (auto dir : {up, left, down, right})
                {
                    p[i] = pos + dir;
                    fringe.emplace_back(cost + 1, p);
                }
            }
        }
    }
    return result;
}

auto parse(auto infile)
{
    std::map<Position, char> nodes;
    int y = 0;
    for (std::string line; std::getline(infile, line); ++y)
        for (int x = 0; x < line.size(); ++x)
            nodes[{x, y}] = line[x];
    auto state = std::set<char>{};
    auto start = Position{};
    for (auto [pos, ch] : nodes)
        if (ch == '@')
            start = pos;
        else if (ch >= 'a' && ch <= 'z')
            state.insert(ch);
    return std::tuple{start, state, nodes};
}

auto part1(auto input)
{
    auto [start, state, nodes] = input;
    return search(nodes, std::vector{start}, state);
}

auto part2(auto input)
{
    auto [start, state, nodes] = input;
    nodes[start + up + left] = nodes[start + up + right] = nodes[start + down + left] = nodes[start + down + right] = '@';
    nodes[start + up] = nodes[start + left] = nodes[start] = nodes[start + right] = nodes[start + down] = '#';
    return search(nodes, std::vector{start + up + left, start + up + right, start + down + left, start + down + right}, state);
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 4042
    std::cout << part2(input) << std::endl; // 2014
    return 0;
}
