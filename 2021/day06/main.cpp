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

auto parse(std::string ages)
{
    std::vector<long> buckets(9, 0l);
    std::stringstream parser(ages);
    for (long age;parser >> age;)
    {
        buckets[age] += 1;
        char delim;
        if (!(parser >> delim)) break;
        assert(delim == ',');
    }
    return buckets;
}

auto simulate(std::vector<long> buckets, int iterations)
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
    return std::accumulate(buckets.begin(), buckets.end(), 0l);
}

auto part1(std::string ages)
{
    return simulate(parse(ages), 80);
}

auto part2(std::string ages)
{
    return simulate(parse(ages), 256);
}

int main()
{
    assert(part1("3,4,3,1,2") == 5934l);
    assert(part2("3,4,3,1,2") == 26984457539l);
    std::ifstream infile("data.txt");
    std::string ages;
    std::getline(infile, ages);
    std::cout << part1(ages) << std::endl; // 380243
    std::cout << part2(ages) << std::endl; // 1708791884591
    return 0;
}