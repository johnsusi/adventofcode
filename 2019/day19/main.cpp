#include <fstream>
#include <iostream>
#include <string>

#include <common/intcode.hpp>

auto scan(auto program, int x0, int y0, int w = 1, int h = 1)
{
    int count = 0;
    for (int y = y0; y < y0 + h; ++y)
        for (int x = x0; x < x0 + w; ++x)
        {
            auto p = program;
            p.push(x);
            p.push(y);
            p.run();
            if (p.pop() == 1)
                ++count;
        }
    return count;
}

auto parse(auto infile)
{
    std::string source;
    std::getline(infile, source);
    return Program{source};
}

auto part1(Program program)
{
    return scan(program, 0, 0, 50, 50);
}

auto part2(Program program)
{
    int x0 = 50, y0 = 0;
    while (!scan(program, x0, y0))
        ++y0;
    y0 += scan(program, x0, y0, 1, 100) - 1;
    for (;; ++x0)
    {
        for (auto h = scan(program, x0, y0, 1, 100); h > 1 && scan(program, x0, y0, 100, 1) < 100; --h)
            ++y0;
        if (scan(program, x0, y0, 1, 100) >= 100)
            return x0 * 10000 + y0;
    }
    return 0;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 234
    std::cout << part2(input) << std::endl; // 9290812
    return 0;
}
