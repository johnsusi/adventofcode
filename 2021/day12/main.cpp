#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <common/string.hpp>

using Node = std::vector<std::string>;

auto parse(std::istream& infile)
{
    std::map<std::string, Node> map;
    for (std::string line; std::getline(infile, line);) {
        auto [from, to] = split(line, "-");
        map[from].push_back(to);
        map[to].push_back(from);
    }
    return map;
}

template <typename F> auto search(const std::map<std::string, Node>& map, F&& check)
{
    int result  = 0;
    auto fringe = std::deque{std::tuple{std::string{"start"}, std::set<std::string>{}}};
    while (!fringe.empty()) {
        auto [cur, visited] = fringe.back();
        fringe.pop_back();
        if (cur == "end") {
            ++result;
            continue;
        }
        if (!check(cur, visited)) continue;
        visited.insert(cur);
        for (auto edge : map.at(cur)) fringe.emplace_back(edge, visited);
    }
    return result;
}

auto part1(std::map<std::string, Node> map)
{
    return search(map, [](auto cur, auto& visited) {
        return !(cur[0] >= 'a' && cur[0] <= 'z' && visited.contains(cur));
    });
}

auto part2(std::map<std::string, Node> map)
{
    return search(map, [](auto cur, auto& visited) {
        if (cur == "start" && visited.contains(cur)) return false;
        if (cur[0] >= 'a' && cur[0] <= 'z' && visited.contains(cur)) {
            if (visited.contains("<twice>")) return false;
            visited.insert("<twice>");
        }
        return true;
    });
}

int main()
{
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 5228
    std::cout << part2(input) << std::endl; // 131228
    return 0;
}