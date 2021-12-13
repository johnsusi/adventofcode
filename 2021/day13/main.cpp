#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <common/string.hpp>

using Node = std::vector<std::string>;

auto parse(std::istream& infile)
{
    std::set<std::pair<int, int>> dots;
    std::vector<std::pair<int, int>> folds;
    int w = 0, h = 0;
    for (std::string line; std::getline(infile, line);) {
        if (line.find(',') == std::string::npos) break;
        auto [xs, ys] = split(line, ",");
        auto x        = std::stoi(xs);
        auto y        = std::stoi(ys);
        w             = std::max(x, w);
        h             = std::max(y, h);
        dots.emplace(x, y);
    }
    for (std::string fold, along, line; infile >> fold >> along >> line;) {
        assert(fold == "fold");
        assert(along == "along");
        auto [axis, offset] = split(line, "=");
        if (axis == "x")
            folds.emplace_back(std::stoi(offset), 0);
        else
            folds.emplace_back(0, std::stoi(offset));
    }
    return std::tuple{dots, folds};
}

enum Fold { foldx, foldy };
auto fold(const std::set<std::pair<int, int>>& dots, Fold fold, int offset)
{
    std::set<std::pair<int, int>> result;
    for (auto [x, y] : dots) {
        if (fold == foldx > 0 && x > offset) x = offset - (x - offset);
        if (fold == foldy > 0 && y > offset) y = offset - (y - offset);
        result.emplace(x, y);
    }
    return result;
}

auto part1(auto input)
{
    auto [dots, folds] = input;
    auto [fold_x, fold_y] = folds[0];
    dots = fold(dots, fold_x > 0 ? foldx : foldy, std::max(fold_x, fold_y));
    return dots.size();
}

auto part2(auto input)
{
    auto [dots, folds] = input;
    for (auto [fold_x, fold_y] : folds)
        dots = fold(dots, fold_x > 0 ? foldx : foldy, std::max(fold_x, fold_y));
    int w = 0, h = 0;
    for (auto [x, y] : dots)
    {
        w = std::max(w, x+1);
        h = std::max(h, y+1);
    }
    std::string result(w*h+h, '.');
    for (int y = 0;y < h;++y)
        result[w + y*w+y] = '\n';
    for (auto [x, y] : dots)
        result[x + y*w + y] = '#';
    return result;
}

int main()
{
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 790
    std::cout << part2(input) << std::endl; // PGHZBFJC
    return 0;
}