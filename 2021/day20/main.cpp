#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>


auto parse(auto infile)
{
    std::string lookup;
    std::getline(infile, lookup);
    std::map<std::pair<int, int>, char> image;
    int w = 0, h = 0;
    for (std::string row; std::getline(infile, row);) {
        if (row.empty()) continue;
        if (w < row.size()) w = row.size();
        for (int x = 0; x < row.size(); ++x) image[{x, h}] = row[x];
        ++h;
    }

    return std::tuple{lookup, image, std::pair{w, h}};
}

auto enhance(const auto& lookup, auto image, int& x1, int& y1, int& x2, int& y2, char empty)
{
    x1 -= 2;
    y1 -= 2;
    x2 += 2;
    y2 += 2;
    for (int i = x1; i <= x2; ++i) {
        image[{i, y1}]     = empty;
        image[{i, y1 + 1}] = empty;
        image[{i, y2}]     = empty;
        image[{i, y2 - 1}] = empty;
    }
    for (int i = y1; i <= y2; ++i) {
        image[{x1, i}]     = empty;
        image[{x1 + 1, i}] = empty;
        image[{x2, i}]     = empty;
        image[{x2 - 1, i}] = empty;
    }

    std::map<std::pair<int, int>, char> temp;

    for (auto [pos, value] : image) {
        auto [x, y] = pos;
        int index   = 0;
        for (int i = -1; i <= 1; ++i)
            for (int j = -1; j <= 1; ++j) {
                index <<= 1;
                char ch = empty;
                if (image.contains({x + j, y + i})) ch = image.at({x + j, y + i});

                if (ch == '#') index |= 1;
            }
        temp[{x, y}] = lookup[index];
    }
    return temp;
}

auto part1(auto input)
{
    auto [lookup, image, size] = input;
    auto [w, h] = size;
    int x1 = 0, y1 = 0, x2 = w - 1, y2 = h - 1;
    char empty = '.';
    for (int i = 0; i < 2; ++i) {
        image = enhance(lookup, image, x1,y1, x2, y2, empty);
        empty = (empty == '.') ? '#' : '.';
    }
    int result = 0;
    for (auto [_, value] : image)
        if (value == '#') result++;
    return result;
}

auto part2(auto input)
{
    auto [lookup, image, size] = input;
    auto [w, h] = size;
    int x1 = 0, y1 = 0, x2 = w - 1, y2 = h - 1;
    char empty = '.';
    for (int i = 0; i < 50; ++i) {
        image = enhance(lookup, image, x1,y1, x2, y2, empty);
        empty = (empty == '.') ? '#' : '.';
    }
    int result = 0;
    for (auto [_, value] : image)
        if (value == '#') result++;
    return result;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 5249
    std::cout << part2(input) << std::endl; // 15714
    return 0;
}
