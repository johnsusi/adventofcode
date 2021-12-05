#include <cassert>
#include <fstream>
#include <iostream>

#include <common/intcode.hpp>

auto test(std::string source, std::string result)
{
    Program program(source);
    program.run();
    return result == program.to_string();
}

auto part1(Program program)
{
    program[1] = 12;
    program[2] = 2;
    program.run();
    return program[0];
}

auto part2(Program program)
{
    for (int noun = 0;noun <= 99;++noun)
        for (int verb = 0;verb <= 99;++verb)
        {
            auto p = program;
            p[1] = noun;
            p[2] = verb;
            p.run();
            if (p[0] == 19690720)
                return 100 * noun + verb;
        }
    return 0;
}

int main()
{
    assert(test("1,0,0,0,99", "2,0,0,0,99"));
    assert(test("2,3,0,3,99", "2,3,0,6,99"));
    assert(test("2,4,4,5,99,0", "2,4,4,5,99,9801"));
    assert(test("1,1,1,4,99,5,6,0,99", "30,1,1,4,2,5,6,0,99"));

    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    Program program(source);
    std::cout << part1(program) << std::endl; // 4090701
    std::cout << part2(program) << std::endl; // 6421
    return 0;
}
