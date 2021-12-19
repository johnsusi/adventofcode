#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <common/string.hpp>


auto operator-(const std::tuple<int, int, int>& lhs, const std::tuple<int, int, int>& rhs)
{
    return std::tuple{std::get<0>(lhs) - std::get<0>(rhs),
                      std::get<1>(lhs) - std::get<1>(rhs),
                      std::get<2>(lhs) - std::get<2>(rhs)};
}

auto detect(const auto& s0, const auto& s1)
{
    for (int i = 0; i < s0.size(); ++i)
        for (int j = 0; j < s1.size(); ++j) {
            auto delta = s1[j] - s0[i];
            int count  = 0;
            for (auto p0 : s0)
                for (auto p1 : s1)
                    if (p0 == (p1 - delta)) {
                        ++count;
                        break;
                    }
            if (count >= 12) return std::pair{count, delta};
        }
    return std::pair{0, std::tuple{0, 0, 0}};
}

auto rotate_x(auto s)
{
    for (auto& [x, y, z] : s) {
        auto t = y;
        y      = -z;
        z      = t;
    }
    return s;
}

auto rotate_y(auto s)
{
    for (auto& [x, y, z] : s) {
        auto t = z;
        z      = -x;
        x      = t;
    }
    return s;
}

auto rotate_z(auto s)
{
    for (auto& [x, y, z] : s) {
        auto t = x;
        x      = -y;
        y      = t;
    }
    return s;
}

auto rotate(auto s, int rx, int ry, int rz)
{
    for (int i = 0; i < rx; ++i) s = rotate_x(s);
    for (int i = 0; i < ry; ++i) s = rotate_y(s);
    for (int i = 0; i < rz; ++i) s = rotate_z(s);
    return s;
}

auto flip_x(auto s)
{
    for (auto& [x, y, z] : s) x = -x;
    return s;
}

auto flip_y(auto s)
{
    for (auto& [x, y, z] : s) y = -y;
    return s;
}

auto flip_z(auto s)
{
    for (auto& [x, y, z] : s) z = -z;
    return s;
}



auto d2(const auto& s0, auto s)
{
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            for (int l = 0; l < 4; ++l) {
                for (int m = 0; m < 2; ++m) {
                    for (int n = 0; n < 2; ++n) {
                        for (int o = 0; o < 2; ++o) {
                            auto [count, pos] = detect(s0, s);
                            if (count >= 12) return std::pair{pos, s};
                            s = flip_z(s);
                        }
                        s = flip_y(s);
                    }
                    s = flip_x(s);
                }
                s = rotate_z(s);
            }
            s = rotate_y(s);
        }
        s = rotate_x(s);
    }
    return std::pair{std::tuple{0, 0, 0}, std::vector<std::tuple<int, int, int>>{}};
}

auto search(auto input)
{
    std::set<int> mapped;
    std::set<std::tuple<int, int, int>> scanners;
    std::set<std::tuple<int, int, int>> beacons;

    for (auto [x, y, z] : input[0]) beacons.emplace(x, y, z);
    mapped.insert(0);
    while (mapped.size() < input.size()) {
        for (int i = 0; i < input.size(); ++i) {
            if (!mapped.contains(i)) continue;
            auto s0 = input[i];
            for (int i2 = 0; i2 < input.size(); ++i2) {
                if (mapped.contains(i2)) continue;
                auto& s        = input[i2];
                auto [pos, s_] = d2(s0, s);
                auto [x, y, z] = pos;
                if (x != 0 || y != 0 || z != 0) {
                    for (auto& [xs, ys, zs] : s_) {
                        xs -= x;
                        ys -= y;
                        zs -= z;
                        beacons.emplace(xs, ys, zs);
                        scanners.emplace(-x, -y, -z);
                    }
                    input[i2] = s_;
                    mapped.insert(i2);
                }
            }
        }
    }
    return std::tuple { scanners, beacons };
}

auto parse(auto infile)
{
    std::vector<std::vector<std::tuple<int, int, int>>> input;
    int index = -1;

    for (std::string s; std::getline(infile, s);) {
        if (s.starts_with("---")) {
            ++index;
            input.resize(input.size() + 1);
        }
        else if (s.empty())
            continue;
        else {
            auto coords = split_all(s, ",");
            assert(coords.size() == 3);
            input[index].emplace_back(
                std::stoi(coords[0]), std::stoi(coords[1]), std::stoi(coords[2]));
        }
    }
    return search(input);
}

auto part1(auto input)
{
    auto [_, beacons] = input;

    return beacons.size();
}

auto part2(auto input)
{
    auto [scanners, _] = input;
    int result = 0;
    for (auto it = scanners.begin(); it != scanners.end(); ++it)
        for (auto jt = it;jt != scanners.end();++jt)
        {
            auto [x0, y0, z0] = *it;
            auto [x1, y1, z1] = *jt;
            result = std::max(result, std::abs(x0 - x1) + std::abs(y0-y1) + std::abs(z0 -z1));
        }
    return result;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 447
    std::cout << part2(input) << std::endl; // 15672
    return 0;
}
