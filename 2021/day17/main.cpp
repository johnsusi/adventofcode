#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <tuple>

auto parse(auto& infile)
{
    std::string s;
    std::getline(infile, s);
    std::regex re("target area: x=(-?\\d+)\\.\\.(-?\\d+), y=(-?\\d+)\\.\\.(-?\\d+)");
    std::smatch m;

    assert(std::regex_search(s, m, re));

    int x1 = std::stoi(m[1]);
    int x2 = std::stoi(m[2]);
    int y1 = std::stoi(m[3]);
    int y2 = std::stoi(m[4]);
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    return std::tuple { x1, y1, x2, y2 };
}

std::optional<int> simulate(auto vx, auto vy, auto x1, auto x2, auto y1, auto y2)
{
    int x = 0, y = 0;
    int ymax = y;
    while (true)
    {
        x += vx;
        y += vy;
        if (vx > 0) --vx;
        else if (vx < 0) ++vx;
        --vy;
        ymax = std::max(ymax, y);
        if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return std::optional { ymax };
        if (x > x2 || y < y1) return std::nullopt;
    }
}

auto part1(auto input)
{
    int result = 0;
    auto [x1, y1, x2, y2] = input;
    for (int vx = 1;vx <= x2;++vx)
        for (int vy = -1000;vy < 1000;++vy)
            if (auto hit = simulate(vx, vy, x1, x2, y1, y2); hit)
                result = std::max(result, *hit);
    return result;
}

auto part2(auto input)
{
    int result = 0;
    auto [x1, y1, x2, y2] = input;
    for (int vx = 1;vx <= x2;++vx)
        for (int vy = -1000;vy < 1000;++vy)
            if (simulate(vx, vy, x1, x2, y1, y2))
                ++result;
    return result;
}

int main()
{
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 6786
    std::cout << part2(input) << std::endl; // 2313
    return 0;
}
