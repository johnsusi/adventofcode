#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto infile)
{
    auto buckets = std::vector<std::int64_t>(9, 0l);
    for (std::int64_t age; infile >> age;)
    {
        buckets[age] += 1;
        if (char delim; !(infile >> delim)) break;
        else assert(delim == ',');
    }
    return buckets;
}

auto simulate(auto buckets, int iterations)
{
    for (int i = 0;i < iterations;++i)
    {
        buckets = std::vector {
            buckets[1],
            buckets[2],
            buckets[3],
            buckets[4],
            buckets[5],
            buckets[6],
            buckets[7] + buckets[0],
            buckets[8],
            buckets[0]
        };
    }
    return std::accumulate(buckets.begin(), buckets.end(), 0ll);
}

auto part1(auto input)
{
    return simulate(input, 80);
}

auto part2(auto input)
{
    return simulate(input, 256);
}

int main()
{
    assert(part1(parse(std::stringstream("3,4,3,1,2"))) == 5934l);
    assert(part2(parse(std::stringstream("3,4,3,1,2"))) == 26984457539ll);
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 380243
    std::cout << part2(input) << std::endl; // 1708791884591
    return 0;
}