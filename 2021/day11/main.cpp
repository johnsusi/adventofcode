#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

template <typename T> auto operator+(const std::pair<T, T>& lhs, const std::pair<T, T>& rhs)
{
    return std::pair{lhs.first + rhs.first, lhs.second + rhs.second};
}

const auto adjacent = std::vector{std::pair{0, -1},
                                  std::pair{-1, -1},
                                  std::pair{-1, 0},
                                  std::pair{-1, 1},
                                  std::pair{0, 1},
                                  std::pair{1, 1},
                                  std::pair{1, 0},
                                  std::pair{1, -1}};

auto parse(std::vector<std::string> input)
{
    std::map<std::pair<int, int>, int> octopuses;
    int x, y = 0;
    for (auto line : input) {
        x = 0;
        for (auto ch : line) { octopuses[{x++, y}] = ch - '0'; }
        ++y;
    }
    return octopuses;
}

auto flash(std::map<std::pair<int, int>, int>& octopuses)
{
    for (auto& [_, energy] : octopuses) ++energy;
    std::set<std::pair<int, int>> flashed;
    while(true) {
        int count = 0;
        for (auto& [pos, energy] : octopuses) {
            if (flashed.contains(pos)) continue;
            if (energy > 9) {
                flashed.insert(pos);
                ++count;
                for (auto rel : adjacent)
                    if (octopuses.contains(pos + rel))
                        octopuses[pos + rel]++;
            }
        }
        for (auto pos : flashed) octopuses[pos] = 0;
        if (count == 0) break;
    }
    return flashed.size();
}

auto part1(std::map<std::pair<int, int>, int> octopuses)
{
    int result = 0;
    for (int i = 0; i < 100; ++i)
        result += flash(octopuses);
    return result;
}

auto part2(std::map<std::pair<int, int>, int> octopuses)
{
    for (int i = 1;; ++i)
        if (flash(octopuses) == octopuses.size()) return i;
    return 0;
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<std::string> input;
    for (std::string line; std::getline(infile, line);) input.push_back(line);
    auto octopuses = parse(input);
    std::cout << part1(octopuses) << std::endl; // 1649
    std::cout << part2(octopuses) << std::endl; // 256
    return 0;
}
