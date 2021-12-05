#include <cassert>
#include <fstream>
#include <iostream>

#include <common/intcode.hpp>

auto test(std::string source, std::vector<std::int64_t> expected)
{
    Program program(source);
    program.run();
    return std::equal(
        program.output.begin(), program.output.end(), expected.begin(), expected.end());
}

auto part1(std::string source)
{
    Program program(source);
    program.push(1);
    program.run();
    return program.pop();
}

auto part2(std::string source)
{
    Program program(source);
    program.push(2);
    program.run();
    return program.pop();
}

int main()
{
    assert(test("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99",
                {109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99}));
    assert(test("1102,34915192,34915192,7,4,7,99,0", {1219070632396864}));
    assert(test("104,1125899906842624,99", {1125899906842624ll}));
    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    std::cout << part1(source) << std::endl; // 2350741403
    std::cout << part2(source) << std::endl; // 53088
    return 0;
}
