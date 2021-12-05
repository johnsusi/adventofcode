#include <cassert>
#include <fstream>
#include <iostream>

#include <common/intcode.hpp>

auto test(std::string source, std::vector<long> input, std::vector<long> output)
{
    Program program(source);
    for (auto x : input)
        program.input.push_back(x);
    program.run();
    for (auto x : output)
    {
        if (program.output.front() != x) return false;
        program.output.pop_front();
    }
    return true;
}

auto part1(Program program)
{
    program.input.push_back(1);
    program.run();
    assert(!program.output.empty());
    auto result = program.output.back();
    program.output.pop_back();
    for (auto x : program.output)
        assert(x == 0);
    return result;
}

auto part2(Program program)
{
    program.input.push_back(5);
    program.run();
    assert(!program.output.empty());
    auto result = program.output.back();
    program.output.pop_back();
    for (auto x : program.output)
        assert(x == 0);
    return result;
}

int main()
{

    assert(test("3,0,4,0,99", { 12345 } , { 12345 }));

    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    Program program(source);
    std::cout << part1(program) << std::endl; // 11933517
    std::cout << part2(program) << std::endl; // 10428568
    return 0;
}
