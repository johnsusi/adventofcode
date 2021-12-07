#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <tuple>

#include <common/intcode.hpp>
#include <common/viz.hpp>

const auto north = std::pair{0, 1};
const auto west  = std::pair{-1, 0};
const auto south = std::pair{0, -1};
const auto east  = std::pair{1, 0};

auto to_dir(int value)
{
    switch (value) {
    case 1: return north;
    case 2: return south;
    case 3: return west;
    case 4: return east;
    default: return std::pair{0, 0};
    }
}

auto turn_back(int value)
{
    return ((value - 1) ^ 1) + 1;
}

struct Node {
    std::pair<int, int> pos;
    std::vector<std::pair<int, int>> trail;
};

auto search(std::string source)
{
    Program program(source);
    auto pos = std::pair{0, 0};
    Map map;
    map[pos] = 'D';
    std::vector<int> trail;

    while (!program.done()) {

        int input = 0;
        if (map[pos + north] == 0)
            input = 1;
        else if (map[pos + west] == 0)
            input = 3;
        else if (map[pos + south] == 0)
            input = 2;
        else if (map[pos + east] == 0)
            input = 4;
        else if (!trail.empty()) {
            input = trail.back();
            trail.pop_back();
        }
        else
            break;

        auto dir = to_dir(input);
        program.push(input);
        program.run();

        auto response = program.pop();
        if (response == 0) { map[pos + dir] = '#'; }
        else if (response == 1) {
            if (map[pos] == 'D') map[pos] = '.';
            if (map[pos + dir] != '.') trail.push_back(turn_back(input));
            map[pos += dir] = 'D';
        }
        else if (response == 2) {
            if (map[pos] == 'D') map[pos] = '.';
            trail.push_back(turn_back(input));
            map[pos += dir] = 'O';
        }
    }
    return map;
}

auto part1(Map map)
{
    std::deque<std::tuple<int, std::pair<int, int>>> fringe;
    fringe.emplace_back(0, std::tuple{0, 0});

    auto is_open = [](int value) { return value != '#' && value != 'X' && value != 0; };

    while (!fringe.empty()) {
        auto [cost, pos] = fringe.front();
        fringe.pop_front();
        if (map[pos] == 'O') return cost;
        map[pos] = 'X';
        if (is_open(map[pos + north])) fringe.emplace_back(cost + 1, pos + north);
        if (is_open(map[pos + west])) fringe.emplace_back(cost + 1, pos + west);
        if (is_open(map[pos + south])) fringe.emplace_back(cost + 1, pos + south);
        if (is_open(map[pos + east])) fringe.emplace_back(cost + 1, pos + east);
    }

    return 0;
}

auto part2(Map map)
{
    std::pair<int, int> pos;
    for (const auto& [key, value] : map.data)
        if (value == 'O')
            pos = key;

    std::deque<std::tuple<int, std::pair<int, int>>> fringe;
    fringe.emplace_back(0, pos);

    auto is_open = [](int value) { return value != '#' &&  value != 'O' && value != 0; };

    int result = 0;
    while (!fringe.empty()) {
        auto [cost, pos] = fringe.front();
        fringe.pop_front();
        map[pos] = 'O';
        result = std::max(result, cost);
        if (is_open(map[pos + north])) fringe.emplace_back(cost + 1, pos + north);
        if (is_open(map[pos + west])) fringe.emplace_back(cost + 1, pos + west);
        if (is_open(map[pos + south])) fringe.emplace_back(cost + 1, pos + south);
        if (is_open(map[pos + east])) fringe.emplace_back(cost + 1, pos + east);
    }

    return result;
}

int main()
{
    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    auto map = search(source);
    std::cout << part1(map) << std::endl; // 224
    std::cout << part2(map) << std::endl; // 284
    return 0;
}
