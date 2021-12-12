#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <common/string.hpp>

struct Node {
    std::string name;
    std::vector<std::string> edges;
    bool operator<(const Node& other) const { return name < other.name; }
};

auto parse(std::istream& infile)
{
    std::map<std::string, Node> map;
    for (std::string line; std::getline(infile, line);) {
        auto [from, to] = split(line, "-");
        map[from].edges.push_back(to);
        map[to].edges.push_back(from);
    }
    return map;
}

auto part1(std::map<std::string, Node> map)
{
    int result = 0;

    auto fringe = std::deque{std::tuple{std::string{"start"}, std::set<std::string>{}}};

    while (!fringe.empty()) {
        auto [cur, visited] = fringe.back();
        fringe.pop_back();

        if (cur == "end") {
            ++result;
            continue;
        }

        if (cur[0] >= 'a' && cur[0] <= 'z')
            if (visited.contains(cur)) continue;

        visited.insert(cur);

        for (auto edge : map[cur].edges) fringe.emplace_back(edge, visited);
    }

    return result;
}

auto part2(std::map<std::string, Node> map)
{
    int result = 0;

    auto fringe = std::deque{std::tuple{std::string{"start"}, std::set<std::string>{}}};

    while (!fringe.empty()) {
        auto [cur, visited] = fringe.back();
        fringe.pop_back();

        if (cur == "end") {
            ++result;
            continue;
        }

        if (cur == "start" && visited.contains(cur)) continue;

        if (cur[0] >= 'a' && cur[0] <= 'z' && visited.contains(cur))
        {
            if (visited.contains("<twice>"))
                continue;
            visited.insert("<twice>");
        }

        visited.insert(cur);

        for (auto edge : map[cur].edges) fringe.emplace_back(edge, visited);
    }

    return result;
}

int main()
{
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 5228
    std::cout << part2(input) << std::endl; // 131228
    return 0;
}