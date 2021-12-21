#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

template <typename T>
auto operator+=(std::pair<T,T>& lhs, const std::pair<T, T>& rhs)
{
    std::get<0>(lhs) += std::get<0>(rhs);
    std::get<1>(lhs) += std::get<1>(rhs);
    return lhs;
}

auto solve(int&p, int& s, int &dice, int& throws)
{
    for (int i = 0;i < 3;++i, ++throws, dice = (dice + 1) % 100)
        p = (p + dice + 1) % 10;
    return s += p + 1;
}

auto solve(const auto &steps, auto &cache, int p1, int s1, int p2, int s2, int step, bool turn1 = false)
{
    if (turn1) {
        p1 = (p1 + step) % 10;
        s1 += p1 + 1;
    }
    else {
        p2 = (p2 + step) % 10;
        s2 += p2 + 1;
    }
    auto key = std::tuple {p1, s1, p2, s2, turn1};
    if (auto it = cache.find(key); it != cache.end())
        return it->second;
    else if (s1 >= 21)
        return cache[key] = {1, 0};
    else if (s2 >= 21)
        return cache[key] = {0, 1};
    auto result = std::pair{0ull, 0ull};
    for (auto step : steps)
        result += solve(steps, cache, p1, s1, p2, s2, step, !turn1);
    return cache[key] = result;
}

auto parse(auto infile)
{
    std::string s1, s2;
    std::getline(infile, s1);
    std::getline(infile, s2);
    return std::pair { std::stoi(s1.substr(28)) - 1, std::stoi(s2.substr(28)) - 1};
}

auto part1(auto input)
{
    auto [p1, p2] = input;
    int dice = 0, throws = 0, score1 = 0, score2 = 0;
    while (true)
    {
        if (solve(p1, score1, dice, throws) >= 1000)
            return score2 * throws;
        if (solve(p2, score2, dice, throws) >= 1000)
            return score1 * throws;
    }
    return 0;
}

auto part2(auto input)
{
    auto [p1, p2] = input;
    auto cache = std::map<std::tuple<int, int, int, int, bool>, std::pair<unsigned long long, unsigned long long>>{};
    std::vector<int> steps;
    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= 3; ++j)
            for (int k = 1; k <= 3; ++k)
                steps.push_back(i+j+k);
    auto [r1, r2] = solve(steps, cache, p1, 0, p2, -p2-1, 0);
    return std::max(r1, r2);
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 853776
    std::cout << part2(input) << std::endl; // 301304993766094
    return 0;
}
