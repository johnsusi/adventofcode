#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

long eval(std::string s, bool advanced)
{
    if (s.find("(") != std::string::npos) {
        std::vector<int> stack;
        for (int i = 0; i < s.size(); ++i) {
            switch (s[i]) {
            case '(': stack.push_back(i); break;
            case ')': {
                int start = stack.back();
                stack.pop_back();
                auto segment     = s.substr(start + 1, i - start - 1);
                std::string head = s.substr(0, start);
                std::string tail = i + 1 >= s.size() ? "" : s.substr(i + 1);
                auto result      = eval(segment, advanced);
                s                = s.substr(0, start) + std::to_string(result) + tail;
                i                = -1;
                stack.clear();
                break;
            }
            default: break;
            }
        }
        return eval(s, advanced);
    }

    std::istringstream parser(s);

    long result;
    parser >> result;

    for (std::string op; parser >> op;) {
        long val;
        if (advanced && op == "*") {
            std::string line;
            std::getline(parser, line);
            val = eval(line, advanced);
            return result * val;
        }
        else
            parser >> val;
        if (op == "+")
            result += val;
        else if (op == "*")
            result *= val;
    }
    return result;
}

auto parse(auto infile)
{
    std::vector<std::string> result;
    for (std::string line; std::getline(infile, line);) result.push_back(line);
    return result;
}

auto part1(auto input)
{
    long sum = 0;
    for (auto line : input) sum += eval(line, false);
    return sum;
}

auto part2(auto input)
{
    long sum = 0;
    for (auto line : input) sum += eval(line, true);
    return sum;
}

int main()
{
    assert(part1(std::vector{"1 + 2 * 3 + 4 * 5 + 6"}) == 71);
    assert(part2(std::vector{"1 + 2 * 3 + 4 * 5 + 6"}) == 231);
    assert(part1(std::vector{"1 + (2 * 3) + (4 * (5 + 6))"}) == 51);
    assert(part2(std::vector{"1 + (2 * 3) + (4 * (5 + 6))"}) == 51);
    assert(part1(std::vector{"2 * 3 + (4 * 5)"}) == 26);
    assert(part2(std::vector{"2 * 3 + (4 * 5)"}) == 46);
    assert(part1(std::vector{"5 + (8 * 3 + 9 + 3 * 4 * 3)"}) == 437);
    assert(part2(std::vector{"5 + (8 * 3 + 9 + 3 * 4 * 3)"}) == 1445);
    assert(part1(std::vector{"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"}) == 12240);
    assert(part2(std::vector{"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"}) == 669060);
    assert(part1(std::vector{"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"}) == 13632);
    assert(part2(std::vector{"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"}) == 23340);
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 15285807527593
    std::cout << part2(input) << std::endl; // 461295257566346
    return 0;
}
