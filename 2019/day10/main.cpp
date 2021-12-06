#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using Position = std::pair<int, int>;

using Edge = std::tuple<double, double, int, int, bool>;

struct Node {
    Position pos;
    std::vector<Edge> edges;
    int visible = 0;
};

auto scan(std::vector<std::string> rows)
{
    std::vector<Node> nodes;
    int y = 0, w = rows.front().size();
    for (auto row : rows) {
        for (int x = 0; x < row.size(); ++x)
            if (row[x] == '#') nodes.push_back({{x, y}});
        ++y;
    }

    int best = 0;
    Node selected;
    for (auto& node1 : nodes) {
        auto [x1, y1] = node1.pos;
        for (auto& node2 : nodes) {
            if (node1.pos != node2.pos) {
                auto [x2, y2] = node2.pos;
                auto dx       = x2 - x1;
                auto dy       = y2 - y1;
                auto theta    = std::atan2(dy, dx) + M_PI / 2.0;
                while (theta < 0) theta += 2.0 * M_PI;
                auto range = std::sqrt(dx * dx + dy * dy);
                node1.edges.emplace_back(theta, range, x2, y2);
            }
        }
        std::set<double> angles;
        for (auto [theta, range, x, y, destroyed] : node1.edges) angles.insert(theta);
        node1.visible = angles.size();
        if (best < node1.visible) {
            best     = node1.visible;
            selected = node1;
        }
    }
    std::sort(selected.edges.begin(), selected.edges.end());
    return selected;
}

auto part1(const Node& node)
{
    return node.visible;
}

auto part2(Node node)
{
    double last = -1;
    int count   = 0;
    while (true) {
        for (auto& [theta, range, x, y, destroyed] : node.edges) {
            if (destroyed || theta == last) continue;
            if ((++count) == 200) return x * 100 + y;
            last      = theta;
            destroyed = true;
        }
    }
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<std::string> rows;
    for (std::string row; std::getline(infile, row);) rows.push_back(row);
    auto nodes = scan(rows);
    std::cout << part1(nodes) << std::endl; // 256
    std::cout << part2(nodes) << std::endl; // 1707
    return 0;
}
