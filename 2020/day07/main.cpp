#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

struct edge_t {
    int count;
    std::string from;
    std::string to;
};

struct node_t {
    std::string name;
    std::vector<edge_t> children;
};

int count_contains(const std::map<std::string, node_t>& nodes, std::string name)
{
    auto contains = [&](const node_t& node) {
        std::deque<node_t> fringe;
        fringe.push_back(node);

        while (!fringe.empty()) {
            auto node = fringe.front();
            fringe.pop_front();
            if (std::find_if(node.children.begin(), node.children.end(), [&name](auto edge) {
                    return edge.to == name;
                }) != node.children.end())
                return true;
            else
                for (auto&& child : node.children) fringe.push_back(nodes.at(child.to));
        }
        return false;
    };

    int count = 0;
    for (auto&& [k, v] : nodes)
        if (k != name && contains(v)) ++count;
    return count;
}

int count_inside(const std::map<std::string, node_t>& nodes, std::string name)
{
    std::deque<std::pair<node_t, int>> fringe;
    fringe.push_back({nodes.at(name), 1});
    int count = 0;
    while (!fringe.empty()) {
        auto [node, amount] = fringe.front();
        fringe.pop_front();
        for (auto&& child : node.children)
        {
          count += amount * child.count;
          fringe.push_back({nodes.at(child.to), amount * child.count});
        }
    }
    return count;
}

int main()
{
    std::ifstream infile("data.txt");

    std::map<std::string, node_t> nodes;
    std::vector<edge_t> edges;
    for (std::string line; std::getline(infile, line);) {
        std::istringstream parser(line);
        std::string attr, color;
        assert(parser >> attr >> color);
        auto name        = attr + " " + color;
        nodes[name].name = name;
        std::string bag, contain;
        assert(parser >> bag >> contain && bag == "bags" && contain == "contain");
        int count;
        while (parser >> count >> attr >> color >> bag) {
            assert(bag == "bag," || bag == "bag." || bag == "bags," || bag == "bags.");
            edges.push_back({count, name, attr + " " + color});
        }
    }

    for (auto& edge : edges) nodes[edge.from].children.push_back(edge);

    std::cout << count_contains(nodes, "shiny gold") << std::endl; // 259
    std::cout << count_inside(nodes, "shiny gold") << std::endl; // 45018
}
