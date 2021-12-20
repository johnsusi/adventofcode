#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

struct rule_t {
    char code = -1;
    std::vector<std::vector<int>> children;
};

bool verify(const auto& rules,
            const auto& message,
            int rule_id          = 0,
            int offset           = 0,
            const std::vector<int>& next = {})
{
    if (offset >= message.size()) return false;
    auto& rule = rules.at(rule_id);
    if (rule.code != -1) {
        if (message[offset] != rule.code) return false;
        if (next.empty()) return offset + 1 == message.size();
        int j = next.front();
        return verify(rules, message, j, offset + 1, {next.begin() +1, next.end()});
    }

    auto chain = [&](std::vector<int> child) {
        std::vector<int> _next(child.begin() + 1, child.end());
        _next.insert(_next.end(), next.begin(), next.end());
        return verify(rules, message, child.front(), offset, _next);
    };

    return std::any_of(rule.children.begin(), rule.children.end(), chain);
}

void parse_rule(auto& rules, const auto& line)
{
    rule_t rule;
    int index;
    std::string delim, token;
    std::istringstream parser(line);
    parser >> index >> delim >> token;
    assert(delim == ":");
    if (token[0] == '\"')
    {
        rule.code = token[1];
        assert(token[2] == '\"' && token.size() == 3);
    }
    else {
      do
      {
        std::vector<int> child;
        child.push_back(std::stoi(token));
        while (parser >> token) {
            if (token == "|") break;
            child.push_back(std::stoi(token));
        }
        rule.children.push_back(child);
      } while (parser >> token);
    }
    rules[index] = rule;
}

auto count(const auto& rules, const auto& messages)
{
    auto is_verified = [&](auto message) { return verify(rules, message); };
    return std::count_if(messages.begin(), messages.end(), is_verified);
};

auto parse(auto infile) {
    std::map<int, rule_t> rules;
    for (std::string line; std::getline(infile, line);) {
        if (line.empty()) break;
        parse_rule(rules, line);
    }

    std::vector<std::string> messages;
    for (std::string line; std::getline(infile, line);) {
        messages.push_back(line);
    }

    return std::pair { rules, messages };
}

auto part1(auto input)
{
    auto [rules, messages] = input;
    return count(rules, messages);
}

auto part2(auto input)
{
    auto [rules, messages] = input;
    parse_rule(rules, "8: 42 | 42 8");
    parse_rule(rules, "11: 42 31 | 42 11 31");
    return count(rules, messages);
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 195
    std::cout << part2(input) << std::endl; // 309
    return 0;
}
