#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> parse(std::string source)
{
    std::stringstream parser(source);
    std::vector<int> result;
    int val;
    char delim;
    while (parser >> val)
    {
        result.push_back(val);
        if (!(parser >> delim)) break;
        assert(delim == ',');
    }
    return result;
}

template <typename F>
auto search(std::string source, F&& cost)
{
    auto pos = parse(source);

    int min = pos.front();
    int max = pos.back();

    for (auto val : pos)
    {
        min = std::min(min, val);
        max = std::max(max, val);
    }

    int best = std::numeric_limits<int>::max();
    for (int i = min;i < max;++i)
    {
        int t = 0;
        for (auto val : pos)
        {
            int n = std::abs(val - i);
            t += cost(n);
        }
        if (t < best) {
            best = t;
        }
    }
    return best;

}

auto part1(std::string source)
{
    return search(source, [](auto n) { return n; });
}

auto part2(std::string source)
{
    return search(source, [](auto n) { return n*(n+1)/2; });
}

int main()
{
    assert(part1("16,1,2,0,4,2,7,1,2,14") == 37);
    assert(part2("16,1,2,0,4,2,7,1,2,14") == 168);
    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    std::cout << part1(source) << std::endl; // 339321
    std::cout << part2(source) << std::endl; // 95476244
    return 0;
}