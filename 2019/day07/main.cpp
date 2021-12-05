#include <cassert>
#include <fstream>
#include <iostream>

#include <common/intcode.hpp>

auto test(std::string source, std::vector<long> input, std::vector<long> output)
{
    Program program(source);
    for (auto x : input) program.input.push_back(x);
    program.run();
    for (auto x : output) {
        if (program.output.front() != x) return false;
        program.output.pop_front();
    }
    return true;
}

auto run(Program program, long x, long s)
{
    program.push(x);
    program.push(s);
    program.run();
    return program.pop();
}

auto part1(std::string source)
{
    Program program(source);
    long result = 0;
    auto phases = std::vector { 0, 1, 2, 3, 4 };
    do {
        long s = 0;
        for (auto phase : phases)
            s = run(program, phase, s);
        if (s > result) result = s;
    } while (std::next_permutation(phases.begin(), phases.end()));
    return result;
}

auto part2(std::string source)
{
    Program program(source);
    long result = 0;
    auto phases = std::vector { 5, 6, 7, 8, 9 };
    do {
        auto programs = std::vector { 5, program };
        for (int i = 0;i < 5;++i)
            programs[i].push(phases[i]);
        programs[0].push(0);

        long s = 0;
        while (!programs[4].done())
        {
            for (int i = 0;i < 5;++i)
            {
                programs[i].run();
                s = programs[i].pop();
                programs[(i+1)% 5].push(s);
            }
        }

        if (s > result) result = s;
    } while (std::next_permutation(phases.begin(), phases.end()));
    return result;
}

int main()
{
    assert(part1("3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0") == 43210);
    assert(part1("3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0") == 54321);
    assert(part1("3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0") == 65210);
    assert(part2("3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5") == 139629729);
    assert(part2("3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10") == 18216);
    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    std::cout << part1(source) << std::endl; // 440880
    std::cout << part2(source) << std::endl; // 3745599
    return 0;
}
