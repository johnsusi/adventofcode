#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <tuple>

auto parse(std::istream& infile)
{
    std::string polymer_template;
    std::getline(infile, polymer_template);
    std::map<std::string, std::string> rules;
    for (std::string a, delim, b; infile >> a >> delim >> b;) {
        assert(delim == "->");
        rules[a] = b;
    }
    return std::tuple { polymer_template, rules };
}

auto score(const auto& counts)
{
    std::uint64_t max = 0, min = std::numeric_limits<std::uint64_t>::max();
    for (auto [key, count] : counts)
    {
        if (count > max) max = count;
        if (count < min) min = count;
    }
    return max - min;
}

auto search(auto polymer_template, const auto& rules, int iterations)
{
    auto pairs = std::map<std::string, std::uint64_t>();
    for (int i = 1;i < polymer_template.size();++i)
        pairs[polymer_template.substr(i-1, 2)]++;
    for (int i = 0;i < iterations;++i)
    {
        decltype(pairs) next;
        for (auto [pair, count] : pairs)
        {
            auto ch = rules.at(pair).front();
            next[{pair[0], ch}] += count;
            next[{ch, pair[1]}] += count;
        }
        pairs = next;
    }
    auto counts = std::map<char, std::uint64_t>{ { polymer_template.back(), 1}};
    for (auto [key, count] : pairs)
        counts[key[0]] += count;
    return score(counts);
}

auto part1(auto input)
{
    auto [polymer_template, rules] = input;
    return search(polymer_template, rules, 10);
}

auto part2(auto input)
{
    auto [polymer_template, rules] = input;
    return search(polymer_template, rules, 40);
}

int main()
{
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 2233
    std::cout << part2(input) << std::endl; // 2884513602164
    return 0;
}
