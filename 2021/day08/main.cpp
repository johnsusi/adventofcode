#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

auto parse(std::string line)
{
    std::vector<std::string> patterns;
    std::vector<std::string> outputs;
    std::stringstream parser(line);
    for (std::string segment; parser >> segment;) {
        if (segment == "|") break;
        patterns.push_back(segment);
    }
    for (std::string segment; parser >> segment;) outputs.push_back(segment);
    return std::pair { patterns, outputs };
}

auto search(const std::vector<std::string>& patterns, const std::vector<std::string>& outputs)
{
    std::map<std::string, int> rosetta;
    rosetta["abcefg"]  = 0; //  aaaa     a = 8x    1 = 2 segments simple
    rosetta["cf"]      = 1; // b    c    b = 6x    7 = 3 segments simple
    rosetta["acdeg"]   = 2; // b    c    c = 8x    4 = 4 segments simple
    rosetta["acdfg"]   = 3; //  dddd     d = 7x    8 = 7 segments simple
    rosetta["bcdf"]    = 4; // e    f    e = 4x    2 = 5 segments
    rosetta["abdfg"]   = 5; // e    f    f = 9x    3 = 5 segments
    rosetta["abdefg"]  = 6; //  gggg     g = 7x    5 = 5 segments
    rosetta["acf"]     = 7; //                     0 = 6 segments
    rosetta["abcdefg"] = 8; //                     6 = 6 segments
    rosetta["abcdfg"]  = 9; //                     9 = 6 segments

    auto lookup = std::string("abcdefg");
    auto decode = [&](std::string s) {
        for (int i = 0; i < s.size(); ++i) s[i] = lookup[s[i] - 'a'];
        std::sort(s.begin(), s.end());
        return s;
    };
    do {
        std::set<int> matches;
        for (auto pattern : patterns) {
            auto s = decode(pattern);
            if (rosetta.contains(s)) matches.insert(rosetta.at(s));
        }
        if (matches.size() == 10) {
            int result = 0;
            for (auto output : outputs) { result = result * 10 + rosetta.at(decode(output)); }
            return result;
        }
    } while (std::next_permutation(lookup.begin(), lookup.end()));
    return 0;
}

auto test(std::string line)
{
    auto [patterns, outputs] = parse(line);
    return search(patterns, outputs);
}

auto part1(const std::vector<int>& inputs)
{
    int result = 0;
    for (auto input : inputs) {
        auto s = std::to_string(input);
        result += std::count_if(s.begin(), s.end(), [](auto x) {
            return x == '1' || x == '4' || x == '7' || x == '8';
        });
    }
    return result;
}

auto part2(const std::vector<int>& inputs)
{
    int result = 0;
    for (auto input : inputs) result += input;
    return result;
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<int> inputs;
    for (std::string line; std::getline(infile, line);)
    {
        auto [patterns, outputs] = parse(line);
        inputs.push_back(search(patterns, outputs));
    }

    assert(test("acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb "
                "cdbaf") == 5353);

    std::cout << part1(inputs) << std::endl; // 381
    std::cout << part2(inputs) << std::endl; // 1023686
    return 0;
}
