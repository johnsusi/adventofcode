#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

std::int64_t f(int w, std::int64_t z, bool a, int b, int c)
{
    int x = (z % 26) + b;
    if (a) z /= 26;
    if (x != w)
        z = z * 26 + w + c;
    return z;
}

auto parse(auto infile)
{
    std::vector<std::tuple<bool, int, int>> input;
    int i = 0, a, b, c;
    for (std::string s; std::getline(infile, s);++i)
    {
        if (s.starts_with("inp ")) { i = a = b = c = 0; }
        else if (i == 4 && s.starts_with("div z ")) a = std::stoi(s.substr(6));
        else if (i == 5 && s.starts_with("add x ")) b = std::stoi(s.substr(6));
        else if (i == 15 && s.starts_with("add y ")) c = std::stoi(s.substr(6));
        else if (i == 17) input.emplace_back(a == 26, b, c);
    }
    return input;
}

auto search(const auto& params, bool reverse = false)
{
    auto ws = std::vector {1, 2, 3, 4, 5, 6, 7, 8, 9};
    if (reverse) std::reverse(ws.begin(), ws.end());
    auto fringe = std::deque { std::tuple { 0, 0ll, 0ull }};
    auto visited = std::set<std::pair<int, std::int64_t>>{};
    while (!fringe.empty())
    {
        auto [i, z, n] = fringe.back();
        fringe.pop_back();
        if (visited.contains({i, z})) continue;
        visited.insert({i, z});
        if (i == 14)
        {
            if (z == 0) return n;
            continue;
        }
        for (auto w : ws)
        {
            auto [a, b, c] = params[i];
            fringe.emplace_back(i+1, f(w, z, a, b, c), n * 10 + w);
        }
    }
    return 0ull;
}

auto part1(const auto& input)
{
    return search(input);
}

auto part2(const auto& input)
{
    return search(input, true);
}

int main()
{
    auto&& input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 98998519596997
    std::cout << part2(input) << std::endl; // 31521119151421
    return 0;
}
