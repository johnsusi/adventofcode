#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <common/intcode.hpp>

auto parse(auto&& infile)
{
    std::string source;
    std::getline(infile, source);
    auto program = Program{source};
    auto nics    = std::vector{50, program};
    for (int i = 0; i < nics.size(); ++i) nics[i].push(i);
    return nics;
}

auto part1(auto nics)
{
    while (true) {
        for (auto& nic : nics) {
            if (!nic.has_input()) nic.push(-1);
            nic.run();
            if (nic.has_output()) {
                auto a = nic.pop();
                auto x = nic.pop();
                auto y = nic.pop();
                if (a == 255)
                    return y;
                else {
                    nics[a].push(x);
                    nics[a].push(y);
                }
            }
        }
    }
}

auto part2(auto nics)
{
    std::uint64_t memx = 0, memy = 0, lasty = 1;
    for (auto& nic : nics) nic.push(-1);
    while (true) {
        int msgs = 0;
        for (auto& nic : nics) {
            if (nic.has_input()) msgs++;
            nic.run();
            if (nic.has_output()) msgs++;
            while (nic.has_output()) {
                auto a = nic.pop();
                auto x = nic.pop();
                auto y = nic.pop();
                if (a == 255) {
                    memx = x;
                    memy = y;
                }
                else {
                    nics[a].push(x);
                    nics[a].push(y);
                }
            }
        }
        if (msgs == 0)
        {
            if (memy == lasty) return lasty;
            nics[0].push(memx);
            nics[0].push(lasty = memy);
            nics[0].run();
        }
    }
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 17286
    std::cout << part2(input) << std::endl; // 11249
    return 0;
}
