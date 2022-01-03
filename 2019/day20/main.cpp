#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

#include <common/string.hpp>

const auto up = std::pair{0, -1};
const auto left = std::pair{-1, 0};
const auto down = std::pair{0, 1};
const auto right = std::pair{1, 0};

auto operator+(auto &&lhs, auto &&rhs)
{
    return std::pair{std::get<0>(lhs) + std::get<0>(rhs), std::get<1>(lhs) + std::get<1>(rhs)};
}

auto to_inner(auto s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) -> unsigned char { return std::tolower(c); });
    return s;
}

auto to_outer(auto s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) -> unsigned char { return std::toupper(c); });
    return s;
}

auto is_inner(auto s)
{
    return s[0] >= 'a' && s[0] <= 'z';
}

auto is_outer(auto s)
{
    return s[0] >= 'A' && s[0] <= 'Z';
}

auto parse(auto infile)
{
    std::map<std::pair<int, int>, std::string> outer_portals, inner_portals;
    std::map<std::pair<int, int>, char> map;

    std::size_t y = 0, w = 0;
    for (std::string line; getline(infile, line); ++y)
    {
        w = std::max(w, line.size());
        if (line[2] == '.')
        {
            auto key = std::string{line[0], line[1]};
            outer_portals[{2, y}] = key;
        }
        int end = line.size() - 1;
        while (line[end] != '.' && line[end] != '#' && end > 0)
            --end;
        if (end > 0 && line[end] == '.')
        {
            auto key = std::string{line[end + 1], line[end + 2]};
            outer_portals[{end, y}] = key;
        }
        for (int x = 0; x < line.size(); ++x)
            map[{x, y}] = line[x];
    }

    for (int x = 2; x < w; ++x)
    {
        if (map[{x, 2}] == '.')
        {
            auto key = std::string{map[{x, 0}], map[{x, 1}]};
            outer_portals[{x, 2}] = key;
        }
        if (map[{x, y - 3}] == '.')
        {
            auto key = std::string{map[{x, y - 2}], map[{x, y - 1}]};
            outer_portals[{x, y - 3}] = key;
        }
    }

    std::map<std::string, std::map<std::string, int>> portals;
    for (auto [from, key] : outer_portals)
    {
        std::set<std::pair<int, int>> visited;
        auto fringe = std::deque{std::tuple{0, from}};
        while (!fringe.empty())
        {
            auto [cost, pos] = fringe.front();
            fringe.pop_front();
            if (visited.contains(pos))
                continue;
            visited.insert(pos);
            for (auto dir : {up, left, down, right})
                if (map[pos + dir] == '.')
                    fringe.emplace_back(cost + 1, pos + dir);
                else if (cost > 0 && map[pos] == '.' && map[pos + dir] >= 'A' && map[pos + dir] <= 'Z')
                {
                    auto dest_key = std::string{map[pos + dir], map[pos + dir + dir]};
                    if (dir == left || dir == up)
                        std::reverse(dest_key.begin(), dest_key.end());
                    if (outer_portals.contains(pos))
                    {
                        if (dest_key != "AA" && key != "ZZ")
                            portals[key][dest_key] = cost;
                    }
                    else
                    {
                        auto inner_key = to_inner(dest_key);
                        portals[key][inner_key] = cost;
                        portals[inner_key][key] = cost;
                        inner_portals.emplace(pos, inner_key);
                    }
                }
        }
    }

    for (auto [from, key] : inner_portals)
    {
        std::set<std::pair<int, int>> visited;
        auto fringe = std::deque{std::tuple{0, from}};
        while (!fringe.empty())
        {
            auto [cost, pos] = fringe.front();
            fringe.pop_front();
            if (visited.contains(pos))
                continue;
            visited.insert(pos);
            for (auto dir : {up, left, down, right})
                if (map[pos + dir] == '.')
                    fringe.emplace_back(cost + 1, pos + dir);
                else if (cost > 0 && map[pos] == '.' && map[pos + dir] >= 'A' && map[pos + dir] <= 'Z')
                {
                    auto dest_key = std::string{map[pos + dir], map[pos + dir + dir]};
                    if (dir == left || dir == up)
                        std::reverse(dest_key.begin(), dest_key.end());
                    if (inner_portals.contains(pos))
                    {
                        auto inner_key = to_inner(dest_key);
                        portals[key][inner_key] = cost;
                        portals[inner_key][key] = cost;
                    }
                }
        }
    }

    for (auto &[from, key] : outer_portals)
    {
        auto inner_key = to_inner(key);
        if (portals.contains(inner_key))
        {
            portals[key][inner_key] = 1;
            portals[inner_key][key] = 1;
        }
    }

    return portals;
}

auto part1(auto input)
{
    auto fringe = std::deque{std::tuple{0ull, std::string{"AA"}}};
    std::set<std::string> visited;
    while (!fringe.empty())
    {
        auto [cost, from] = fringe.front();
        fringe.pop_front();
        if (visited.contains(from))
            continue;
        visited.insert(from);
        if (from == "ZZ")
            return cost;
        for (auto [to, edge_cost] : input[from])
            fringe.emplace_back(cost + edge_cost, to);
    }
    return 0ull;
}

auto part2(auto input)
{
    std::set<std::pair<int, std::string>> visited;
    for (auto [key, _] : input)
        if (is_outer(key) && key != "AA" && key != "ZZ")
            visited.emplace(0, key);
    auto fringe = std::deque{std::tuple{0, 0, std::string{"AA"}}};
    auto result = std::numeric_limits<int>::max();
    while (!fringe.empty())
    {
        auto [cost, level, from] = fringe.front();
        fringe.pop_front();
        if (visited.contains({level, from}))
            continue;
        if (cost >= result)
            continue;
        visited.emplace(level, from);
        if (level == 0 && from == "ZZ")
            return cost;
        for (auto [to, edge_cost] : input[from])
        {
            if (is_inner(from) && to == to_outer(from))
                fringe.emplace_back(cost + edge_cost, level + 1, to);
            else if (is_outer(from) && to == to_inner(from))
                fringe.emplace_back(cost + edge_cost, level - 1, to);
            else
                fringe.emplace_back(cost + edge_cost, level, to);
        }
    }
    return 0;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 632
    std::cout << part2(input) << std::endl; // 7162
    return 0;
}
