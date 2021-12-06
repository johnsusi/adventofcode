#include <algorithm>
#include <fstream>
#include <iostream>

#include <common/intcode.hpp>

auto part1(Program program)
{
    program.run();
    int result = 0;
    while (program.has_output()) {
        auto x = program.pop();
        auto y = program.pop();
        auto t = program.pop();
        if (t == 2) result++;
    }
    return result;
}

auto part2(Program program)
{
    program[0] = 2;
    int paddle = 0, ball = 0, score = 0;
    while (!program.done()) {
        auto input = std::clamp(ball - paddle, -1, 1);
        program.push(input);
        program.run();
        while (program.has_output()) {
            auto x = program.pop();
            auto y = program.pop();
            auto t = program.pop();
            if (x == -1 && y == 0)
                score = t;
            else {
                if (t == 3) paddle = x;
                if (t == 4) ball = x;
            }
        }
    }
    return score;
}

int main()
{
    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    auto program = Program{source};
    std::cout << part1(program) << std::endl; // 326
    std::cout << part2(program) << std::endl; // 15988
    return 0;
}
