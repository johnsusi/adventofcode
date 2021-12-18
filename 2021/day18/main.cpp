#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto infile)
{
    std::vector<std::vector<std::pair<int, int>>> lines;
    for (std::string s; std::getline(infile, s);) {
        int nest = 0;
        auto line = decltype(lines)::value_type{};
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '[')
                ++nest;
            else if (s[i] == ']')
                --nest;
            else if (s[i] >= '0' && s[i] <= '9')
                line.emplace_back(nest, s[i] - '0');
        }
        lines.push_back(line);
    }
    return lines;
}

void explode(auto& line)
{
    for (int i = 0; i < line.size();) {
        auto& [nest, value] = line[i];
        if (nest == 5) {
            if (i > 0) {
                auto& [_, _value] = line[i - 1];
                _value += value;
            }
            auto [xnest, xvalue] = line[i + 1];
            if (i + 2 < line.size()) {
                auto& [_, _value] = line[i + 2];
                _value += xvalue;
            }
            line.erase(line.begin() + i + 1);
            nest -= 1;
            value = 0;
            i     = 0;
        }
        else
            i++;
    }
}

auto split(auto& line)
{
    for (int i = 0; i < line.size(); ++i) {
        auto& [nest, value] = line[i];
        if (value < 10) continue;
        auto lhs = value / 2;
        value -= lhs;
        nest++;
        line.emplace(line.begin() + i, nest, lhs);
        return true;
    }
    return false;
}

auto reduce(auto line)
{
    do {
        explode(line);
    } while (split(line));
    return line;
}

auto add(auto lhs, auto rhs)
{
    for (auto& [nest, _] : lhs) nest++;
    for (auto& [nest, _] : rhs) nest++;
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    return lhs;
}

auto magnitude(auto line)
{
    while (line.size() > 1) {
        std::vector<std::pair<int, int>> temp;
        for (int i = 1; i < line.size(); ++i) {
            auto [a, b] = line[i - 1];
            auto [c, d] = line[i];
            if (a == c) {
                temp.emplace_back(a - 1, 3 * b + 2 * d);
                ++i;
            }
            else
                temp.emplace_back(a, b);
        }
        line = temp;
    }
    return line.front().second;
}

auto part1(auto input)
{
    auto line = input[0];
    for (int i = 1; i < input.size(); ++i) line = reduce(add(line, input[i]));
    return magnitude(line);
}

auto part2(auto input)
{
    int result = 0;
    for (int i = 0; i < input.size(); ++i)
        for (int j = 0; j < input.size(); ++j) {
            if (i == j) continue;
            auto line  = reduce(add(input[i], input[j]));
            auto score = magnitude(line);
            result     = std::max(score, result);
        }
    return result;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 3699
    std::cout << part2(input) << std::endl; // 4735
    return 0;
}
