#include <fstream>
#include <iostream>

#include <common/intcode.hpp>

auto parse(auto&& infile)
{
    std::string source;
    std::getline(infile, source);
    return Program{source};
}

auto part1(Program program)
{
    program.push("NOT C J\n");
    program.push("AND D J\n");
    program.push("NOT A T\n");
    program.push("OR T J\n");
    program.push("WALK\n");
    program.run();
    return program.output.back();
}

auto part2(auto program)
{
    program.push("NOT B J\n");
    program.push("NOT C T\n");
    program.push("OR T J\n");
    program.push("AND D J\n");
    program.push("AND H J\n");
    program.push("NOT A T\n");
    program.push("OR T J\n");
    program.push("RUN\n");
    program.run();
    return program.output.back();
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 19355391
    std::cout << part2(input) << std::endl; // 1143770635
    return 0;
}
