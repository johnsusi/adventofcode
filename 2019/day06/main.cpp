#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <common/string.hpp>

struct Node
{
    std::string name;
    std::vector<std::string> edges;
    std::string parent;
    bool operator==(const Node& other) const { return name == other.name; }
    bool operator!=(const Node& other) const { return name != other.name; }
    bool operator<(const Node& other) const { return name < other.name; }
};

auto part1(const std::map<std::string, Node>& nodes)
{
    int result = 0;
    auto root = nodes.at("COM");
    auto fringe = std::deque { std::pair { root, 0 } };

    while (!fringe.empty())
    {
        auto [node, depth] = fringe.front();
        fringe.pop_front();
        result += depth;
        for (auto edge : node.edges)
        {
            auto child = nodes.at(edge);
            fringe.emplace_back(child, depth+1);
        }
    }
    return result;
}

auto part2(const std::map<std::string, Node>& nodes)
{
    auto com = nodes.at("COM");
    auto you = nodes.at("YOU");
    auto san = nodes.at("SAN");

    std::vector<std::string> a, b;

    for (auto node = nodes.at(you.parent);node != com;node = nodes.at(node.parent))
        a.push_back(node.name);

    for (auto node = nodes.at(san.parent);node != com;node = nodes.at(node.parent))
        b.push_back(node.name);

    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());
    auto [it, jt] = std::mismatch(a.begin(), a.end(), b.begin());
    return std::distance(it, a.end()) + std::distance(jt, b.end());
}

int main()
{
    std::ifstream infile("data.txt");
    std::map<std::string, Node> nodes;

    for (std::string line;std::getline(infile, line);)
    {
        auto edge = split(line, ")");
        nodes[edge.first].name = edge.first;
        nodes[edge.first].edges.push_back(edge.second);
        nodes[edge.second].name = edge.second;
        nodes[edge.second].parent = edge.first;
    }

    std::cout << part1(nodes) << std::endl; // 110190
    std::cout << part2(nodes) << std::endl; // 343

    return 0;
}
