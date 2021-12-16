#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

auto score (char c) {
    switch (c) {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default: return 0;
    }
}

auto is_open (char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

auto is_close (char open, char c) {
    switch (open) {
        case '(': return c == ')';
        case '[': return c == ']';
        case '{': return c == '}';
        case '<': return c == '>';
        default: return false;
    }
}

auto checksum(std::string s)
{
    std::string a;
    for (char ch : s)
    {
        if (is_open(ch))
            a.push_back(ch);
        else if (is_close(a.back(), ch))
        {
            a.pop_back();
        }
        else return std::tuple { false, ch, a };
    }
    return std::tuple { true, '\0', a };
}

auto score_completion(std::string s)
{
    std::uint64_t score = 0;
    for (char ch : s)
        switch (ch)
        {
            case '(': score = score * 5 + 1; break;
            case '[': score = score * 5 + 2; break;
            case '{': score = score * 5 + 3; break;
            case '<': score = score * 5 + 4; break;
        }
    return score;
}

auto part1(std::vector<std::string> input)
{
    int result = 0;
    for (auto s : input)
    {
        auto [ok, ch, _] = checksum(s);
        if (!ok) result += score(ch);
    }
    return result;
}

auto part2(std::vector<std::string> input)
{
    std::vector<std::uint64_t> scores;
    for (auto s : input)
    {
        auto [ok, ch, tail] = checksum(s);
        if (!ok || tail.empty()) continue;
        std::reverse(tail.begin(), tail.end());
        scores.push_back(score_completion(tail));
    }
    std::sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<std::string> input;
    for (std::string line; std::getline(infile, line);) input.push_back(line);
    std::cout << part1(input) << std::endl; // 271245
    std::cout << part2(input) << std::endl; // 1685293086
    return 0;
}
